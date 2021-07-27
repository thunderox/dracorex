
#include "dracorex_synth.h"
#include "chorus.h"

int number_of_voices = 12;
voice* voices[12];
int current_voice = 0;
int next_voice = 0;
int keys[128];
int sampleRate;

void dracorex_synth_init(int sample_rate)
{

	wavetable_set_sample_rate(sample_rate);
	sampleRate = sample_rate;

	for (int x=0; x<128; x++)
	{
		keys[x] = -1;
	}


	for (int x=0; x<12; x++)
	{
		voices[x] = (voice*)malloc(sizeof(voice));	
		voices[x]->frequency = 0;
		voices[x]->active = false;

		for (int y=0; y<4; y++)
		{
			voices[x]->osc[y].wave1_number = 3;	
			voices[x]->osc[y].wave2_number = 1;
			voices[x]->osc[y].wave_mix = 0;
			voices[x]->osc[y].start_phase = true;
			voices[x]->osc[y].frequency = 0;
			voices[x]->osc[y].index = 0;
			voices[x]->osc[y].increment = 1;
			voices[x]->osc[y].bandlimit_offset = 0;
		}

		// Init filter left

		voices[x]->buf0_left=0; voices[x]->buf1_left=0;

	  	voices[x]->bf0_left=0; voices[x]->bf1_left=0; voices[x]->bf2_left=0; voices[x]->bf3_left=0; voices[x]->bf4_left=0;  //filter buffers (beware denormals!)
	  	voices[x]->t1_left=0; voices[x]->t2_left=0;              //temporary buffers

		voices[x]->selectivity_left=90, voices[x]->gain1_left=0, voices[x]->gain2_left=0.25, voices[x]->ratio_left=7, voices[x]->cap_left=0; 

		// Init filter right

		voices[x]->buf0_right=0; voices[x]->buf1_right=0;

		voices[x]->f_right=0; voices[x]->pc_right=0; voices[x]->q_right=0;             //filter coefficients
	  	voices[x]->bf0_right=0; voices[x]->bf1_right=0; voices[x]->bf2_right=0; voices[x]->bf3_right=0; voices[x]->bf4_right=0;  //filter buffers (beware denormals!)
	  	voices[x]->t1_right=0; voices[x]->t2_right=0;              //temporary buffers

		voices[x]->selectivity_right=90, voices[x]->gain1_right=0, voices[x]->gain2_right=0.25, voices[x]->ratio_right=7, voices[x]->cap_right=0; 


		voices[x]->env_amp_state = ENV_STATE_DORMANT;
		voices[x]->env_amp_level = 0;

		voices[x]->env_wave_state = ENV_STATE_DORMANT;
		voices[x]->env_wave_level = 0;

		voices[x]->env_adsr3_state = ENV_STATE_DORMANT;
		voices[x]->env_adsr3_level = 0;

		chorus_init();
	}

	wavetable_initialise();

	FILE *ptr_file;
	int x;
	printf("\n");


	ptr_file =fopen("waves/wave_files.txt", "r");
	fseek(ptr_file, 0L, SEEK_END);
	int length = ftell(ptr_file);
	fseek(ptr_file, 0, SEEK_SET);
	char wave_file_buffer[length];

	int wave_number = 0;

	while (fgets(wave_file_buffer,length, ptr_file) != NULL)
	{
		for (int x=0; x<length; x++)
		{
			if (wave_file_buffer[x] == ',') wave_file_buffer[x] = 0;
		}

		if (wave_file_buffer[0] != 10)
		{
			if (wave_number<16)
				wavetable_make_wave(wave_number, dracorex_self->bundle_path, wave_file_buffer);
			wave_number++;
		} else break;
	}

	fclose(ptr_file);
}

void dracorex_synth_cleanup()
{
	for (int x=0; x<12; x++)
	{
		free(voices[x]);		
	}
}

void dracorex_synth_play(float* const out_left, float* const out_right, uint32_t n_frames)
{

	float master_volume = dracorex_self->params[dracorex_VOLUME][0]; 

	float amp_attack = fast_pow(1-dracorex_self->params[dracorex_AMP_ATTACK][0],10); 
	float amp_decay = fast_pow(1-dracorex_self->params[dracorex_AMP_DECAY][0],10);
	float amp_sustain = dracorex_self->params[dracorex_AMP_SUSTAIN][0];
	float amp_release = fast_pow(1-dracorex_self->params[dracorex_AMP_RELEASE][0],10);

	float wave_attack = fast_pow(1-dracorex_self->params[dracorex_WAVE_ATTACK][0],10); 
	float wave_decay = fast_pow(1-dracorex_self->params[dracorex_WAVE_DECAY][0],10);
	float wave_sustain = dracorex_self->params[dracorex_WAVE_SUSTAIN][0];
	float wave_release = fast_pow(1-dracorex_self->params[dracorex_WAVE_RELEASE][0],10);

	float adsr3_attack = fast_pow(1-dracorex_self->params[dracorex_ADSR3_ATTACK][0],10); 
	float adsr3_decay = fast_pow(1-dracorex_self->params[dracorex_ADSR3_DECAY][0],10);
	float adsr3_sustain = dracorex_self->params[dracorex_ADSR3_SUSTAIN][0];
	float adsr3_release = fast_pow(1-dracorex_self->params[dracorex_ADSR3_RELEASE][0],10);
		
	float osc1_detune = dracorex_self->params[dracorex_OSC1_TUNING][0];
	float osc2_detune = dracorex_self->params[dracorex_OSC2_TUNING][0];

	bool osc1_active = dracorex_self->params[dracorex_OSC1_ACTIVE][0];	
	bool osc2_active = dracorex_self->params[dracorex_OSC2_ACTIVE][0];

	bool filter_active = dracorex_self->params[dracorex_FILTER_ACTIVE][0];
	float filter_cutoff = dracorex_self->params[dracorex_CUTOFF][0];	
	float filter_res = dracorex_self->params[dracorex_RESONANCE][0];

	int osc1_waveA_number = dracorex_self->params[dracorex_OSC1_WAVE_A][0];
	int osc1_waveB_number = dracorex_self->params[dracorex_OSC1_WAVE_B][0];
	int osc2_waveA_number = dracorex_self->params[dracorex_OSC2_WAVE_A][0];
	int osc2_waveB_number = dracorex_self->params[dracorex_OSC2_WAVE_B][0];

	bool chorus_active = dracorex_self->params[dracorex_CHORUS_ACTIVE][0];

	float lfo1_wave = dracorex_self->params[dracorex_LFO1_WAVE][0]; 
	float lfo1_speed = dracorex_self->params[dracorex_LFO1_SPEED][0] / ((sampleRate / 4410)*60);
	float lfo1_retrig = dracorex_self->params[dracorex_LFO1_RETRIG][0]; 
	float lfo1_osc1_amount = dracorex_self->params[dracorex_LFO1_OSC1_AMOUNT][0]; 
	float lfo1_osc2_amount = dracorex_self->params[dracorex_LFO1_OSC2_AMOUNT][0]; 
	float lfo1_osc1_wave_amount = dracorex_self->params[dracorex_LFO1_OSC1_WAVE_AMOUNT][0]; 
	float lfo1_osc2_wave_amount = dracorex_self->params[dracorex_LFO1_OSC2_WAVE_AMOUNT][0]; 
	float lfo1_filter_amount = dracorex_self->params[dracorex_LFO1_FILTER_AMOUNT][0]; 

	float lfo2_wave = dracorex_self->params[dracorex_LFO2_WAVE][0]; 
	float lfo2_speed = dracorex_self->params[dracorex_LFO2_SPEED][0] / ((sampleRate / 4410)*60);
	float lfo2_retrig = dracorex_self->params[dracorex_LFO2_RETRIG][0]; 
	float lfo2_osc1_amount = dracorex_self->params[dracorex_LFO2_OSC1_AMOUNT][0]; 
	float lfo2_osc2_amount = dracorex_self->params[dracorex_LFO2_OSC2_AMOUNT][0]; 
	float lfo2_osc1_wave_amount = dracorex_self->params[dracorex_LFO2_OSC1_WAVE_AMOUNT][0]; 
	float lfo2_osc2_wave_amount = dracorex_self->params[dracorex_LFO2_OSC2_WAVE_AMOUNT][0]; 
	float lfo2_filter_amount = dracorex_self->params[dracorex_LFO2_FILTER_AMOUNT][0]; 

	float osc1_pitch_adsr2 = dracorex_self->params[dracorex_OSC1_PITCH_ADSR2][0];
	float osc1_pitch_adsr3 = dracorex_self->params[dracorex_OSC1_PITCH_ADSR3][0];
	float osc2_pitch_adsr2 = dracorex_self->params[dracorex_OSC2_PITCH_ADSR2][0];
	float osc2_pitch_adsr3 = dracorex_self->params[dracorex_OSC2_PITCH_ADSR3][0];

	for (int fr=0; fr<n_frames; fr++)
	{
		float out = 0;

		for (int x=0; x<12; x++)
		{


			if ( voices[x]->active)
			{

				float in = out;

				//--------- ADSR AMP
				// ATTACK

				switch (voices[x]->env_amp_state)
				{


				case ENV_STATE_ATTACK:
					if (voices[x]->env_amp_level < 1) 
					{
						voices[x]->env_amp_level += amp_attack;
						if (voices[x]->env_amp_level >= 1)
						{
							voices[x]->env_amp_level = 1;
							voices[x]->env_amp_state = ENV_STATE_DECAY;
						}
					}
					break;

				// DECAY / SUSTAIN

				case ENV_STATE_DECAY:
				{	
					if (voices[x]->env_amp_level > amp_sustain) 
					{
						voices[x]->env_amp_level -= amp_decay;
							if (voices[x]->env_amp_level <= amp_sustain)
							{
								voices[x]->env_amp_level = amp_sustain;
								voices[x]->env_amp_state = ENV_STATE_WAIT;
							}
					}

					if (voices[x]->env_amp_level <amp_sustain) 
					{
						voices[x]->env_amp_level += amp_decay;
							if (voices[x]->env_amp_level >=amp_sustain)
							{
								voices[x]->env_amp_level = amp_sustain;
								voices[x]->env_amp_state = ENV_STATE_WAIT;
							}
					}
					break;

					// RELEASE

					case ENV_STATE_RELEASE:
						if (voices[x]->env_amp_level > 0) 
						{
							voices[x]->env_amp_level -=amp_release;
							if (voices[x]->env_amp_level <= 0)
							{
								voices[x]->env_amp_level = 0;
								voices[x]->env_amp_state = ENV_STATE_DORMANT;
								voices[x]->active = false; 	
							}
						}
						break;
					}
				}
			
					
			float env_amp_level_db = (voices[x]->env_amp_level * voices[x]->env_amp_level * voices[x]->env_amp_level) * master_volume; 

				//--------- ADSR WAVE
				// ATTACK

				switch (voices[x]->env_wave_state)
				{


				case ENV_STATE_ATTACK:
					if (voices[x]->env_wave_level < 1) 
					{
						voices[x]->env_wave_level += wave_attack;
						if (voices[x]->env_wave_level >= 1)
						{
							voices[x]->env_wave_level = 1;
							voices[x]->env_wave_state = ENV_STATE_DECAY;
						}
					}
					break;

				// DECAY / SUSTAIN

				case ENV_STATE_DECAY:
				{	
					if (voices[x]->env_wave_level > wave_sustain) 
					{
						voices[x]->env_wave_level -= wave_decay;
							if (voices[x]->env_wave_level <= wave_sustain)
							{
								voices[x]->env_wave_level = wave_sustain;
								voices[x]->env_wave_state = ENV_STATE_WAIT;
							}
					}

					if (voices[x]->env_wave_level <wave_sustain) 
					{
						voices[x]->env_wave_level += wave_decay;
							if (voices[x]->env_wave_level >=wave_sustain)
							{
								voices[x]->env_wave_level = wave_sustain;
								voices[x]->env_wave_state = ENV_STATE_WAIT;
							}
					}
					break;

					// RELEASE

					case ENV_STATE_RELEASE:
						if (voices[x]->env_wave_level > 0) 
						{
							voices[x]->env_wave_level -= wave_release;
							if (voices[x]->env_wave_level <= 0)
							{
								voices[x]->env_wave_level = 0;
								voices[x]->env_wave_state = ENV_STATE_DORMANT;
							}
						}
						break;
					}
				}

				// Do Filter Envelope


				// ATTACK

				switch (voices[x]->env_adsr3_state)
				{


				case ENV_STATE_ATTACK:
					if (voices[x]->env_adsr3_level < 1) 
					{
						voices[x]->env_adsr3_level += adsr3_attack;
						if (voices[x]->env_adsr3_level >= 1)
						{
							voices[x]->env_adsr3_level = 1;
							voices[x]->env_adsr3_state = ENV_STATE_DECAY;
						}
					}
					break;

				// DECAY / SUSTAIN

				case ENV_STATE_DECAY:
				{	
					if (voices[x]->env_adsr3_level > adsr3_sustain) 
					{
						voices[x]->env_adsr3_level -= adsr3_decay;
							if (voices[x]->env_adsr3_level <= adsr3_sustain)
							{
								voices[x]->env_adsr3_level = adsr3_sustain;
								voices[x]->env_adsr3_state = ENV_STATE_WAIT;
							}
					}

					if (voices[x]->env_adsr3_level <adsr3_sustain) 
					{
						voices[x]->env_adsr3_level += adsr3_decay;
							if (voices[x]->env_adsr3_level >=adsr3_sustain)
							{
								voices[x]->env_adsr3_level = adsr3_sustain;
								voices[x]->env_adsr3_state = ENV_STATE_WAIT;
							}
					}
					break;

					// RELEASE

					case ENV_STATE_RELEASE:
						if (voices[x]->env_adsr3_level > 0) 
						{
							voices[x]->env_adsr3_level -=adsr3_release;
							if (voices[x]->env_adsr3_level <= 0)
							{
								voices[x]->env_adsr3_level = 0;
								voices[x]->env_adsr3_state = ENV_STATE_DORMANT;
							}
						}
						break;
					}
				}
				
				// ----------------	
	
			
				float env_wave_level_db = (voices[x]->env_wave_level * voices[x]->env_wave_level * voices[x]->env_wave_level);

				voices[x]->osc[2].wave1_number = lfo1_wave;
				voices[x]->osc[3].wave1_number = lfo2_wave;

				voices[x]->osc[2].frequency = lfo1_speed;
				float lfo1_out = wavetable_tick( &voices[x]->osc[2]);
 
				voices[x]->osc[3].frequency = lfo2_speed;
				float lfo2_out = wavetable_tick( &voices[x]->osc[3]);


				voices[x]->osc[0].wave_mix = voices[x]->env_wave_level * (1+(lfo1_out * lfo1_osc1_wave_amount)) * (1+(lfo2_out * lfo2_osc1_wave_amount));	
				voices[x]->osc[1].wave_mix = voices[x]->env_wave_level * (1+(lfo1_out * lfo1_osc2_wave_amount)) * (1+(lfo2_out * lfo2_osc2_wave_amount));
				voices[x]->osc[0].wave1_number = osc1_waveA_number;
				voices[x]->osc[0].wave2_number = osc1_waveB_number;
				voices[x]->osc[1].wave1_number = osc2_waveA_number;
				voices[x]->osc[1].wave2_number = osc2_waveB_number;


				if (voices[x]->osc[0].start_phase) 
				{

					float osc1_freq = voices[x]->frequency + osc1_detune;
					osc1_freq += (12*lfo1_osc1_amount) * lfo1_out;
					osc1_freq += (12*lfo2_osc1_amount) * lfo2_out;
					osc1_freq += (12*osc1_pitch_adsr2) * (voices[x]->env_wave_level);
					osc1_freq += (12*osc1_pitch_adsr3) * (voices[x]->env_adsr3_level);
					voices[x]->osc[0].frequency = fastishP2F(osc1_freq) * 0.8175;

					if (voices[x]->osc[0].frequency > 440) voices[x]->osc[0].frequency = 440;
					voices[x]->osc[0].start_phase = false;
				}	

				if (voices[x]->osc[1].start_phase) 
				{
					float osc2_freq = voices[x]->frequency + osc2_detune;
					osc2_freq += (12*lfo1_osc2_amount) * lfo1_out;
					osc2_freq += (12*lfo2_osc2_amount) * lfo2_out;
					osc2_freq += (12*osc2_pitch_adsr2) * (voices[x]->env_wave_level);
					osc2_freq += (12*osc2_pitch_adsr3) * (voices[x]->env_adsr3_level);
					voices[x]->osc[1].frequency = fastishP2F(osc2_freq) * 0.8175;
 
					if (voices[x]->osc[1].frequency > 440) voices[x]->osc[1].frequency = 440;
					voices[x]->osc[1].start_phase = false;
				}	


				if (osc1_active) in = (wavetable_tick( &voices[x]->osc[0] ) * 0.5) * env_amp_level_db;
				if (osc2_active) in +=  (wavetable_tick( &voices[x]->osc[1]) * 0.5) * env_amp_level_db;	


				if (filter_active)
				{

					// Lowpass  output:  bf4
					// Highpass output:  out - bf4;
					// Bandpass output:  3.0f * (bf3 - bf4);

					// Moog 24 dB/oct resonant lowpass VCF
					// References: CSound source code, Stilson/Smith CCRMA paper.
					// Modified by paul.kellett@maxim.abel.co.uk July 2000

					// Set coefficients given filter_cutoff & filter_res [0.0...1.0]

					float freq = filter_cutoff * voices[x]->env_adsr3_level * (1+(lfo1_filter_amount*(3*lfo1_out))) * (1+(lfo2_filter_amount*(3*lfo2_out)));

					if (freq<0) {freq = 0;}
					if (freq>1) {freq = 1;}
	
					// voices[x]->gain1_left = 1.0/(voices[x]->selectivity_left + 1.0);

					// voices[x]->cap_left = (in + voices[x]->cap_left * voices[x]->selectivity_left ) * voices[x]->gain1_left;
					// in = ((in + voices[x]->cap_left * voices[x]->ratio_left) * voices[x]->gain2_left);
	
					voices[x]->q_left = 1.0f - freq;
					voices[x]->pc_left = freq + 0.8f * freq * voices[x]->q_left;
					voices[x]->f_left = voices[x]->pc_left + voices[x]->pc_left - 1.0f;
					voices[x]->q_left = filter_res * (1.0f + 0.5f * voices[x]->q_left * (1.0f - voices[x]->q_left + 5.6f * voices[x]->q_left * voices[x]->q_left));
	
					// Filter (in [-1.0...+1.0])

					in -= voices[x]->q_left * voices[x]->bf4_left;                          //feedback
					voices[x]->t1_left = voices[x]->bf1_left;  voices[x]->bf1_left = (in + voices[x]->bf0_left) * voices[x]->pc_left - voices[x]->bf1_left * voices[x]->f_left;
					voices[x]->t2_left = voices[x]->bf2_left;  voices[x]->bf2_left = (voices[x]->bf1_left + voices[x]->t1_left) * voices[x]->pc_left - voices[x]->bf2_left * voices[x]->f_left;
					voices[x]->t1_left = voices[x]->bf3_left;  voices[x]->bf3_left = (voices[x]->bf2_left + voices[x]->t2_left) * voices[x]->pc_left - voices[x]->bf3_left * voices[x]->f_left;
					voices[x]->bf4_left = (voices[x]->bf3_left + voices[x]->t1_left) * voices[x]->pc_left - voices[x]->bf4_left * voices[x]->f_left;
					voices[x]->bf4_left = voices[x]->bf4_left - voices[x]->bf4_left * voices[x]->bf4_left * voices[x]->bf4_left * 0.166667f;    //clipping
					voices[x]->bf0_left = in;
	
					// Lowpass  output:  bf4
					// Highpass output:  out - bf4;
					// Bandpass output:  3.0f * (bf3 - bf4);


					out += voices[x]->bf4_left;

				} else out += in;
			}
		}



		if (chorus_active)
		{	
			float* chorus_result = chorus(out, out);
			out_left[fr] = chorus_result[0];
			out_right[fr] = chorus_result[1];
		}
		else
		{
			out_left[fr] = out;
			out_right[fr] = out;
		}
	}
}



void dracorex_synth_note_on(int note, int volume)
{

	next_voice = -1;

	for (int x=0; x<number_of_voices; x++)
	{
		if (voices[x]->active == false)
		{
			next_voice = x;
			break;
		}
	}
	
	if (next_voice==-1) // Bugger we're using all voices, steal the quietest one that's no longer in attack 
	{
		int quietest_note = -1;
		float quietest_level = 2; 
		for (int x=0; x<number_of_voices; x++)
		{
			if (voices[x]->env_amp_level < quietest_level && voices[x]->env_amp_state != ENV_STATE_ATTACK)
			{
				quietest_note = x;
				quietest_level = voices[x]->env_amp_level;
			}
		}

		next_voice = quietest_note;
	}


	keys[note] = next_voice;
	current_voice = next_voice;

	voices[current_voice]->active = true;
	voices[current_voice]->env_amp_state = ENV_STATE_ATTACK;
	voices[current_voice]->env_wave_state = ENV_STATE_ATTACK;
	voices[current_voice]->env_adsr3_state = ENV_STATE_ATTACK;
        voices[current_voice]->env_amp_level = 0;
	voices[current_voice]->env_wave_level = 0;
	voices[current_voice]->env_adsr3_level = 0;
	voices[current_voice]->frequency = note;
	voices[current_voice]->volume = (float)volume/128 ;
	voices[current_voice]->osc->index == 4410;
	voices[current_voice]->osc[0].frequency = fastishP2F(note + dracorex_self->params[dracorex_OSC1_TUNING][0]);
	voices[current_voice]->osc[1].frequency = fastishP2F(note + dracorex_self->params[dracorex_OSC2_TUNING][0]);
}


void dracorex_synth_note_off(int note)
{
	for (int x=0; x<number_of_voices; x++)
	{
		if ( voices[x]->active && keys[note]==x )
		{
			voices[x]->env_amp_state = ENV_STATE_RELEASE;
			voices[x]->env_wave_state = ENV_STATE_RELEASE;
			voices[x]->env_adsr3_state = ENV_STATE_RELEASE;
			keys[note] = -1;
		}
	}
}


//----------------------------------------------------------------------------------------

int dracorex_synth_get_number_of_voices_playing()
{
	int number_of_voices_playing = 0;

	for (int v=0; v<12; v++)
	{
		if (voices[v]->active) number_of_voices_playing++;
	}	

	return number_of_voices_playing;
}

//----------------------------------------------------------------------------------------

void dracorex_synth_set_osc_waves(int osc, int wave1, int wave2)
{
	for (int v=0; v<12; v++)
	{
		voices[v]->osc[osc].wave1_number = wave1;
		voices[v]->osc[osc].wave2_number = wave2;
	}
}







