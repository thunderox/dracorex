
#include "wavetable.h"
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define M_PI 3.14159265358979323846

const int wavetable_number_of_waves = 16;
wave* waves[16];
int wavetable_sample_rate = 44100;
float tuning = 0;

float buf0,buf1;
float f, pc, q;             //filter coefficients
float bf0, bf1, bf2, bf3, bf4;  //filter buffers (beware denormals!)
float t1, t2;              //temporary buffers
float selectivity, gain1, gain2, ratio, cap; 

float filter_frequency,filter_resonance;



//-----------------------------------------------------------
// Make waveform

void wavetable_make_wave(int wave_number, const char* path, const char* filename)
{
	int length = waves[wave_number]->length;

	float* buffer_source_wave = (float *)malloc(12*(length*sizeof(float)) );

	if (buffer_source_wave == NULL) { printf("Failed to allocate memory!\n"); return; }

	float* buffer_filtered_wave = (float *)malloc(12*(length*sizeof(float)) );

	if (buffer_filtered_wave == NULL) { printf("Failed to allocate memory!\n"); return; }

	// Load waveform


	FILE *ptr_myfile;

	char* filename_and_path = (char*) malloc( strlen(path) + strlen(filename) + 12);

	sprintf(filename_and_path, "%swaves/%s.wav", path, filename);
	printf("Loading and converting - %s", filename_and_path);
	printf("\n");


	ptr_myfile = fopen(filename_and_path,"rb");

	if (ptr_myfile)
	{

		for (int x=0; x<8; x++)
		{	
			fseek(ptr_myfile, 80, SEEK_SET);
			fread(&buffer_source_wave[x*length] ,1, length*sizeof(float), ptr_myfile);
		}


		fclose(ptr_myfile);

		//------ Filter

		// Moog 24 dB/oct resonant lowpass VCF
		// References: CSound source code, Stilson/Smith CCRMA paper.
		// Modified by paul.kellett@maxim.abel.co.uk July 2000

		// Set coefficients given frequency & resonance [0.0...1.0]
	
		float frequency = 0.04;
		float resonance = 0;	
		float in;

		memcpy( &waves[wave_number]->buffer[0], &buffer_source_wave[0], length*sizeof(4));

		for (int wave=1; wave<8; wave++)
		{

			float minpeak = 0;
			float maxpeak = 0;
			float maxvol = 0;

			for (int x=0; x<length*8; x++)
			{

				in = buffer_source_wave[x];

				q = 1.0f - frequency;
				pc = frequency + 0.8f * frequency * q;
				f = pc + pc - 1.0f;
				q = resonance * (1.0f + 0.5f * q * (1.0f - q + 5.6f * q * q));
	
				// Filter (in [-1.0...+1.0])

				in -= q * bf4;                          //feedback
				t1 = bf1;  bf1 = (in + bf0) * pc - bf1 * f;
				t2 = bf2;  bf2 = (bf1 + t1) * pc - bf2 * f;
				t1 = bf3;  bf3 = (bf2 + t2) * pc - bf3 * f;
				bf4 = (bf3 + t1) * pc - bf4 * f;
				bf4 = bf4 - bf4 * bf4 * bf4 * 0.166667f;    //clipping
				bf0 = in;


				buffer_filtered_wave[x] = bf4;

	
					if (bf4 < 0 && bf4 < minpeak) minpeak = bf4;
					if (bf4 > 0 && bf4 > maxpeak) maxpeak = bf4;
			

			

			}

			if (-minpeak > maxpeak) maxvol = -minpeak;
				else maxvol = maxpeak;

			if (-minpeak == maxpeak) maxvol = maxpeak;

	
	
			float amp = 0.7/maxvol;


			for (int y=0; y<length-1; y++)	// Normalise
			{
				waves[wave_number]->buffer[y+(wave*length)] = buffer_filtered_wave[y+(4*4110)]*amp;
			}


			frequency /= 1.8;
		}

	} else printf("\nFile not found!\n\n"); 


	free(buffer_source_wave);
	free(buffer_filtered_wave);
	free(filename_and_path);
}




//-----------------------------------------------------------
// Play wavetable 

float wavetable_tick(oscillator* osc)
{
	int wave1_number = osc->wave1_number;
	int wave2_number = osc->wave2_number;
	float wave_mix = osc->wave_mix;

	float frequency = osc->frequency * tuning;
	int length = waves[wave1_number]->length;

	float increment = osc->increment;

	float index = osc->index;
	int index_int = index;

	float result1 = 0, result2 = 0;

	unsigned int wavnum = osc->bandlimit_offset;

	if (index == index_int) 
	{
		result1 = waves[wave1_number]->buffer[wavnum+index_int];
		result2 = waves[wave2_number]->buffer[wavnum+index_int];		
	}
	else
	{
		float fraction1 = index - index_int;
		float fraction2 = 1 - fraction1;



		if (index < length-2)
		{	

			result1 =  waves[wave1_number]->buffer[index_int+wavnum] * fraction2;
			result1 += waves[wave1_number]->buffer[index_int+1+wavnum] * fraction1;
			result2 =  waves[wave2_number]->buffer[index_int+wavnum] * fraction2;
			result2 += waves[wave2_number]->buffer[index_int+1+wavnum] * fraction1;	
		}
		else 
		{
			result1 =  waves[wave1_number]->buffer[index_int+wavnum] * fraction2;
			result1 += waves[wave1_number]->buffer[wavnum] * fraction1;
			result2 =  waves[wave2_number]->buffer[index_int+wavnum] * fraction2;
			result2 += waves[wave2_number]->buffer[wavnum] * fraction1;
		}
	}	

	index += increment;

	if (index > length - 1 )
	{	
		index -= length;	
		osc->start_phase = true;

		int tst = (frequency/100)*8;
		if (tst<0) tst = 0;
		if (tst>7) tst = 7;
		osc->bandlimit_offset = (tst*4410);
	}


	osc->index = index;	

	osc->increment = (wavetable_sample_rate * frequency) / wavetable_sample_rate;

	return (result2 * wave_mix) + (result1 * (1-wave_mix));

}

float* wavetable_get_buffer(int wave_number)
{
	return waves[wave_number]->buffer;
}




//-----------------------------------------------------------
// Set frequency

void wavetable_set_oscillator_frequency(oscillator* osc,float freq)
{
	osc->frequency = freq;
}

//-----------------------------------------------------------
// Set oscillator wave number

void wavetable_set_oscillator_wave_number(oscillator* osc,int wave_number, int wave_number2)
{
	osc->wave1_number = wave_number;
	osc->wave2_number = wave_number2;
}

//-----------------------------------------------------------
// Set oscillator wave number

void wavetable_set_oscillator_wave_mix(oscillator* osc,float wave_mix)
{
	osc->wave_mix = wave_mix;
}

//-------------------------------------------------------------
// Set sample rate

void wavetable_set_sample_rate(int sample_rate)
{
	wavetable_sample_rate = sample_rate;
	tuning = (float)44100 / sample_rate;	
	
	printf("Tuning - %f", tuning);
	
}

//-------------------------------------------------------------
// Initialise

void wavetable_initialise()
{
	for (int x=0; x<wavetable_number_of_waves; x++)
	{
		waves[x] = (wave *)malloc(sizeof(wave));
		waves[x]->length = 4410;
		waves[x]->buffer = (float *)malloc(8*4410*sizeof(float));
	}

	// Init anti alias filter

	buf0=0; buf1=0;

	f=0; pc=0; q=0;             //filter coefficients
  	bf0=0; bf1=0; bf2=0; bf3=0; bf4=0;  //filter buffers (beware denormals!)
  	t1=0; t2=0;              //temporary buffers

	selectivity=90, gain1=0, gain2=0.25, ratio=7, cap=0; 
	
	return;
}

//-------------------------------------------------------------
// Cleanup

void wavetable_cleanup()
{
	for (int x=0; x<wavetable_number_of_waves; x++)
	{
		free(waves[x]->buffer);
		free(waves[x]);
	}



}


