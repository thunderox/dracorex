
#include "oscillator.hpp"

//---------------------------------------------------------------------------------------------------

oscillator::oscillator()
{
	start_phase = true;
	frequency = 1;
	index = 0;
	increment = 1;
	bandlimit_offset = 0;
	sample_rate = 44100;
}

//---------------------------------------------------------------------------------------------------

oscillator::~oscillator()
{

}


//-----------------------------------------------------------
// Play wavetable 

float oscillator::tick()
{

	tuning = 1;
	int index_int = index;
	
	if (frequency < 0.01) frequency = 0.01;
	
	float result1 = 0, result2 = 0, result3 = 0, result4 = 0;
	
	int wsz = wavetable.size()-1;
	
	if (wave_a < 0) wave_a = 0;
	if (wave_a > wsz) wave_a = wsz;
	
	if (wave_b < 0) wave_b = 0;
	if (wave_b > wsz) wave_b = wsz;
	
	float* w_a1 = wavetable[wave_a];
	float* w_b1 = wavetable[wave_b];

	float* w_a2;
	float* w_b2;
	
	if (wave_a < wsz)
		w_a2 = wavetable[wave_a + 1];
	else
		w_a2 = wavetable[wave_a];
	
	if (wave_b < wsz)
		w_b2 = wavetable[wave_b + 1];
	else
		w_b2 = wavetable[wave_b];

	


	if (index == index_int) 
	{
		result1 = w_a1[bandlimit_offset+index_int];
		result2 = w_b1[bandlimit_offset+index_int];	
		result3 = w_a2[bandlimit_offset+index_int];
		result4 = w_b2[bandlimit_offset+index_int];	
	}
	else
	{
		float fraction1 = index - index_int;
		float fraction2 = 1 - fraction1;

		if (index < length-1)
		{	

			result1 =  w_a1[index_int+bandlimit_offset] * fraction2;
			result1 += w_a1[index_int+1+bandlimit_offset] * fraction1;
			result2 =  w_b1[index_int+bandlimit_offset] * fraction2;
			result2 += w_b1[index_int+1+bandlimit_offset] * fraction1;	
			
			result3 =  w_a2[index_int+bandlimit_offset] * fraction2;
			result3 += w_a2[index_int+1+bandlimit_offset] * fraction1;
			result4 =  w_b2[index_int+bandlimit_offset] * fraction2;
			result4 += w_b2[index_int+1+bandlimit_offset] * fraction1;
		}
		else 
		{
			result1 =  w_a1[index_int+bandlimit_offset] * fraction2;
			result1 += w_a1[bandlimit_offset] * fraction1;
			result2 =  w_b1[index_int+bandlimit_offset] * fraction2;
			result2 += w_b1[bandlimit_offset] * fraction1;
			
			result3 =  w_a2[index_int+bandlimit_offset] * fraction2;
			result3 += w_a2[bandlimit_offset] * fraction1;
			result4 =  w_b2[index_int+bandlimit_offset] * fraction2;
			result4 += w_b2[bandlimit_offset] * fraction1;
		}
	}	

	index += increment;

	if (index > length - 1 )
	{	
		index -= length;	
		start_phase = true;

		int bandlimit_number = (note / 10);
		if (bandlimit_number<0) bandlimit_number = 0;
		if (bandlimit_number>11) bandlimit_number = 11;
		bandlimit_offset = (bandlimit_number*length);
	}

	increment = (44100 * frequency) / sample_rate;
	
	float wave_a_mod_mix = wave_a - int(wave_a);
	float o1 = (result3 * wave_a_mod_mix) + (result1 * (1 - wave_a_mod_mix) ); 

	float wave_b_mod_mix = wave_b - int(wave_b);
	float o2 = (result4 * wave_b_mod_mix) + (result2 * (1 - wave_b_mod_mix) ); 

	return (o2 * wave_mix) + (o1 * (1-wave_mix) );

}





	
	
	
