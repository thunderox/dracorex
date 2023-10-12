
#include "voice.hpp"

//----------------------------------------------------------------------------------------------------------------

voice::voice()
{
	amp_env.state = ENV_STATE_DORMANT;
	amp_env.level = 0;
	wave_env.state = ENV_STATE_DORMANT;
	wave_env.level = 0;
	amp2_env.state = ENV_STATE_DORMANT;
	amp2_env.level = 0;
	wave2_env.state = ENV_STATE_DORMANT;
	wave2_env.level = 0;
	adsr3_env.state = ENV_STATE_DORMANT;
	adsr3_env.level = 0;
	adsr4_env.state = ENV_STATE_DORMANT;
	adsr4_env.level = 0;
	
	active = false;

};

//----------------------------------------------------------------------------------------------------------------

voice::~voice()
{

};

//------------------------------------------------------------------------------------------------
// APPROX POW

double fast_pow(double a, double b) {
  union {
    double d;
    int x[2];
  } u = { a };
  u.x[1] = (int)(b * (u.x[1] - 1072632447) + 1072632447);
  u.x[0] = 0;
  return u.d;
}


//===============================================================
float fastishP2F (float pitch)
{
	long convert;
	float *p=(float *)&convert;
	float fl,fr,warp,out;

	pitch *=0.0833333; //pitch scaling. remove this line for pow(2,a)
	fl = floor(pitch);
	fr = pitch - fl;
	float fr2 = fr*fr;
	warp = fr*0.696 + fr2*0.225 + fr*fr2*0.079;  // chebychev approx
	//warp = fr*0.65 + fr*fr*0.35; // chebychev approx

	out = fl+warp;
	out *= 8388608.0; //2^23;
	out += 127.0 * 8388608.0; //2^23;
	convert = out; //magic

	return *p;
}


//----------------------------------------------------------------------------------------------------------------
// PLAY VOICE

float voice::play(float* left_buffer, float* right_buffer,  uint32_t frames)
{

	if (!active) return 0;

	float amp_attack = fast_pow(fParameters[dracorex_AMP_ATTACK],10); 
	float amp_decay = fast_pow(fParameters[dracorex_AMP_DECAY],10);
	float amp_sustain = 1-fParameters[dracorex_AMP_SUSTAIN];
	float amp_release = fast_pow(fParameters[dracorex_AMP_RELEASE],10);
	
	float wave_attack = fast_pow(fParameters[dracorex_WAVE_ATTACK],10); 
	float wave_decay = fast_pow(fParameters[dracorex_WAVE_DECAY],10);
	float wave_sustain = 1-fParameters[dracorex_WAVE_SUSTAIN];
	float wave_release = fast_pow(fParameters[dracorex_WAVE_RELEASE],10);
	
	float amp2_attack = fast_pow(fParameters[dracorex_AMP2_ATTACK],10); 
	float amp2_decay = fast_pow(fParameters[dracorex_AMP2_DECAY],10);
	float amp2_sustain = 1-fParameters[dracorex_AMP2_SUSTAIN];
	float amp2_release = fast_pow(fParameters[dracorex_AMP2_RELEASE],10);
	
	float wave2_attack = fast_pow(fParameters[dracorex_WAVE2_ATTACK],10); 
	float wave2_decay = fast_pow(fParameters[dracorex_WAVE2_DECAY],10);
	float wave2_sustain = 1-fParameters[dracorex_WAVE2_SUSTAIN];
	float wave2_release = fast_pow(fParameters[dracorex_WAVE2_RELEASE],10);
			
	float adsr3_attack = fast_pow(fParameters[dracorex_ADSR3_ATTACK],10); 
	float adsr3_decay = fast_pow(fParameters[dracorex_ADSR3_DECAY],10);
	float adsr3_sustain = 1-fParameters[dracorex_ADSR3_SUSTAIN];
	float adsr3_release = fast_pow(fParameters[dracorex_ADSR3_RELEASE],10);
	
	float adsr4_attack = fast_pow(fParameters[dracorex_ADSR4_ATTACK],10); 
	float adsr4_decay = fast_pow(fParameters[dracorex_ADSR4_DECAY],10);
	float adsr4_sustain = 1-fParameters[dracorex_ADSR4_SUSTAIN];
	float adsr4_release = fast_pow(fParameters[dracorex_ADSR4_RELEASE],10);

	for (uint32_t x=0; x<frames; x++)
	{
	
		//--------- ADSR AMP ------------------------------------------------------------------------------------
		// ATTACK

		switch (amp_env.state)
		{
			case ENV_STATE_ATTACK:
			if (amp_env.level < 1) 
			{
				amp_env.level += amp_attack;
				if (amp_env.level >= 1)
				{
					amp_env.level = 1;
					amp_env.state = ENV_STATE_DECAY;
				}
			}
			break;

			// DECAY / SUSTAIN
		
			case ENV_STATE_DECAY:
			{	
				if (amp_env.level > amp_sustain) 
				{
					amp_env.level -= amp_decay;
					if (amp_env.level <= amp_sustain)
					{
						amp_env.level = amp_sustain;
						amp_env.state = ENV_STATE_WAIT;
					}
				}

				if (amp_env.level < amp_sustain) 
				{
					amp_env.level += amp_decay;
					if (amp_env.level >= amp_sustain)
					{
						amp_env.level = amp_sustain;
						amp_env.state = ENV_STATE_WAIT;
					}
				}
			}
			break;

			// RELEASE

			case ENV_STATE_RELEASE:
			if (amp_env.level > 0) 
			{
				amp_env.level -= amp_release;
				if (amp_env.level <= 0)
				{
					amp_env.level = 0;
					amp_env.state = ENV_STATE_DORMANT;	
				}
			}
			break;
		}
		
		
		//--------- ADSR WAVE ------------------------------------------------------------------------------------
		// ATTACK

		switch (wave_env.state)
		{
			case ENV_STATE_ATTACK:
			if (wave_env.level < 1) 
			{
				wave_env.level += wave_attack;
				if (wave_env.level >= 1)
				{
					wave_env.level = 1;
					wave_env.state = ENV_STATE_DECAY;
				}
			}
			break;

			// DECAY / SUSTAIN
		
			case ENV_STATE_DECAY:
			{	
				if (wave_env.level > wave_sustain) 
				{
					wave_env.level -= wave_decay;
					if (wave_env.level <= wave_sustain)
					{
						wave_env.level = wave_sustain;
						wave_env.state = ENV_STATE_WAIT;
					}
				}

				if (wave_env.level < wave_sustain) 
				{
					wave_env.level += wave_decay;
					if (wave_env.level >= wave_sustain)
					{
						wave_env.level = wave_sustain;
						wave_env.state = ENV_STATE_WAIT;
					}
				}
			}
			break;

			// RELEASE

			case ENV_STATE_RELEASE:
			if (wave_env.level > 0) 
			{
				wave_env.level -= wave_release;
				if (wave_env.level <= 0)
				{
					wave_env.level = 0;
					wave_env.state = ENV_STATE_DORMANT;
				}
			}
			break;
		}
				
				
		//--------- ADSR AMP 2 -------------------------------------------------------------------------------------
		// ATTACK

		switch (amp2_env.state)
		{
			case ENV_STATE_ATTACK:
			if (amp2_env.level < 1) 
			{
				amp2_env.level += amp2_attack;
				if (amp2_env.level >= 1)
				{
					amp2_env.level = 1;
					amp2_env.state = ENV_STATE_DECAY;
				}
			}
			break;

			// DECAY / SUSTAIN
		
			case ENV_STATE_DECAY:
			{	
				if (amp2_env.level > amp_sustain) 
				{
					amp2_env.level -= amp2_decay;
					if (amp2_env.level <= amp2_sustain)
					{
						amp2_env.level = amp2_sustain;
						amp2_env.state = ENV_STATE_WAIT;
					}
				}

				if (amp2_env.level < amp2_sustain) 
				{
					amp2_env.level += amp2_decay;
					if (amp2_env.level >= amp2_sustain)
					{
						amp2_env.level = amp2_sustain;
						amp2_env.state = ENV_STATE_WAIT;
					}
				}
			}
			break;

			// RELEASE

			case ENV_STATE_RELEASE:
				if (amp2_env.level > 0) 
				{
					amp2_env.level -= amp2_release;
					if (amp2_env.level <= 0)
					{
						amp2_env.level = 0;
						amp2_env.state = ENV_STATE_DORMANT;
					}
				}
				break;
		}
		
		
		//--------- ADSR WAVE 2 ------------------------------------------------------------------------------------
		// ATTACK

		switch (wave2_env.state)
		{
			case ENV_STATE_ATTACK:
			if (wave2_env.level < 1) 
			{
				wave2_env.level += wave2_attack;
				if (wave2_env.level >= 1)
				{
					wave2_env.level = 1;
					wave2_env.state = ENV_STATE_DECAY;
				}
			}
			break;

			// DECAY / SUSTAIN
		
			case ENV_STATE_DECAY:
			{	
				if (wave2_env.level > wave2_sustain) 
				{
					wave2_env.level -= wave2_decay;
					if (wave2_env.level <= wave2_sustain)
					{
						wave2_env.level = wave2_sustain;
						wave2_env.state = ENV_STATE_WAIT;
					}
				}

				if (wave2_env.level < wave2_sustain) 
				{
					wave2_env.level += wave2_decay;
					if (wave2_env.level >= wave2_sustain)
					{
						wave2_env.level = wave_sustain;
						wave2_env.state = ENV_STATE_WAIT;
					}
				}
			}
			break;

				
			// RELEASE

			case ENV_STATE_RELEASE:
			if (wave2_env.level > 0) 
			{
				wave2_env.level -= wave2_release;
				if (wave2_env.level <= 0)
				{
					wave2_env.level = 0;
					wave2_env.state = ENV_STATE_DORMANT;
				}
			}
			break;
		}
		
		
		//--------- ADSR 3 ------------------------------------------------------------------------------------
		// ATTACK

		switch (adsr3_env.state)
				{
		case ENV_STATE_ATTACK:
			if (adsr3_env.level < 1) 
			{
				adsr3_env.level += adsr3_attack;
				if (adsr3_env.level >= 1)
				{
					adsr3_env.level = 1;
					adsr3_env.state = ENV_STATE_DECAY;
				}
			}
			break;

			// DECAY / SUSTAIN
			
			case ENV_STATE_DECAY:
			{	
				if (adsr3_env.level > adsr3_sustain) 
				{
					adsr3_env.level -= adsr3_decay;
					if (adsr3_env.level <= adsr3_sustain)
					{
						adsr3_env.level = adsr3_sustain;
						adsr3_env.state = ENV_STATE_WAIT;
					}
				}

				if (adsr3_env.level < adsr3_sustain) 
				{
					adsr3_env.level += adsr3_decay;
					if (adsr3_env.level >= adsr3_sustain)
					{
						adsr3_env.level = adsr3_sustain;
						adsr3_env.state = ENV_STATE_WAIT;
					}
				}
			}
			break;

			// RELEASE

			case ENV_STATE_RELEASE:
				if (adsr3_env.level > 0) 
				{
					adsr3_env.level -= adsr3_release;
					if (adsr3_env.level <= 0)
					{
						adsr3_env.level = 0;
						adsr3_env.state = ENV_STATE_DORMANT;
					}
				}
			break;
			}
					
		//--------- ADSR 4 ------------------------------------------------------------------------------------
		// ATTACK

		switch (adsr4_env.state)
				{
		case ENV_STATE_ATTACK:
			if (adsr4_env.level < 1) 
			{
				adsr4_env.level += adsr4_attack;
				if (adsr4_env.level >= 1)
				{
					adsr4_env.level = 1;
					adsr4_env.state = ENV_STATE_DECAY;
				}
			}
			break;

			// DECAY / SUSTAIN
			
			case ENV_STATE_DECAY:
			{	
				if (adsr4_env.level > adsr4_sustain) 
				{
					adsr4_env.level -= adsr4_decay;
					if (adsr4_env.level <= adsr4_sustain)
					{
						adsr4_env.level = adsr4_sustain;
						adsr4_env.state = ENV_STATE_WAIT;
					}
				}

				if (adsr4_env.level < adsr4_sustain) 
				{
					adsr4_env.level += adsr4_decay;
					if (adsr4_env.level >= adsr4_sustain)
					{
						adsr4_env.level = adsr4_sustain;
						adsr4_env.state = ENV_STATE_WAIT;
					}
				}
			}
			break;

			// RELEASE

			case ENV_STATE_RELEASE:
				if (adsr4_env.level > 0) 
				{
					adsr4_env.level -= adsr4_release;
					if (adsr4_env.level <= 0)
					{
						adsr4_env.level = 0;
						adsr4_env.state = ENV_STATE_DORMANT;
					}
				}
			break;
			}
		
		//float env_amp_level_db = (amp_env.level * amp_env.level * amp_env.level) * master_volume; 

		osc1.wave_mix = wave_env.level;
		osc2.wave_mix = wave2_env.level;
				
		float osc1_out;
		float osc2_out;
		
		if (fParameters[dracorex_OSC1_ACTIVE]) osc1_out = osc1.tick() * fParameters[dracorex_OSC1_VOLUME]; else osc1_out = 0;
		if (fParameters[dracorex_OSC2_ACTIVE]) osc2_out = osc2.tick() * fParameters[dracorex_OSC2_VOLUME]; else osc2_out = 0;		

		float in_left;
		float in_right;

		// DO FILTER
		
		if (fParameters[dracorex_FILTER_ACTIVE])
		{
			// DO FILTER LEFT
					
			float frequency = fParameters[dracorex_CUTOFF] + ( fParameters[dracorex_FILTER_ADSR3_AMOUNT] * adsr3_env.level );
			float resonance = fParameters[dracorex_RESONANCE];		
			
			if (frequency > 1) frequency = 1;
		
			in_left = osc1_out * (amp_env.level * fParameters[dracorex_VOLUME] )
				+ osc2_out * (amp2_env.level * fParameters[dracorex_VOLUME] );

			q_left = 1.0f - frequency;
			pc_left = frequency + 0.8f * frequency * q_left;
			f_left= pc_left + pc_left - 1.0f;
			q_left = resonance * (1.0f + 0.5f * q_left * (1.0f - q_left + 5.6f * q_left * q_left));

			in_left  -= q_left * bf4_left;                          //feedback
			t1_left = bf1_left;  bf1_left = (in_left + bf0_left) * pc_left - bf1_left * f_left;
			t2_left = bf2_left;  bf2_left = (bf1_left + t1_left) * pc_left - bf2_left * f_left;
			t1_left = bf3_left;  bf3_left = (bf2_left + t2_left) * pc_left - bf3_left * f_left;
			bf4_left = (bf3_left + t1_left) * pc_left - bf4_left * f_left;
			bf4_left = bf4_left - bf4_left * bf4_left * bf4_left * 0.166667f;    //clipping
			bf0_left = in_left;
			in_left = bf4_left;
			
			if (  isnan(bf4_right) ) {bf1_right = 0; bf2_right = 0; bf3_right = 0; bf4_right = 0; in_right = 0;}
			
			// DO FILTER RIGHT

			in_right = osc1_out * (amp_env.level * fParameters[dracorex_VOLUME] )
				+ osc2_out * (amp2_env.level * fParameters[dracorex_VOLUME] );

			q_right = 1.0f - frequency;
			pc_right = frequency + 0.4f * frequency * q_right;
			f_right= pc_right + pc_right - 1.0f;
			q_right = resonance * (1.0f + 0.5f * q_right * (1.0f - q_right + 5.6f * q_right * q_right));
			
			in_right  -= q_right * bf4_right;                          //feedback
			t1_right = bf1_right;  bf1_right = (in_right + bf0_right) * pc_right - bf1_right * f_right;
			t2_right = bf2_right;  bf2_right = (bf1_right + t1_right) * pc_right - bf2_right * f_right;
			t1_right = bf3_right;  bf3_right = (bf2_right + t2_right) * pc_right - bf3_right * f_right;
			bf4_right = (bf3_right + t1_right) * pc_right - bf4_right * f_right;
			bf4_right = bf4_right - bf4_right * bf4_right * bf4_right * 0.166667f;    //clipping
			bf0_right = in_right;
			in_right = bf4_right;
			
			if (  isnan(bf4_left) ) {bf1_left = 0; bf2_left = 0; bf3_left = 0; bf4_left = 0; in_left = 0;}
		
		} 
		else
		{
			in_left = osc1_out * (amp_env.level * fParameters[dracorex_VOLUME] )
				+ osc2_out * (amp2_env.level * fParameters[dracorex_VOLUME] );
				
			in_right = osc1_out * (amp_env.level * fParameters[dracorex_VOLUME] )
				+ osc2_out * (amp2_env.level * fParameters[dracorex_VOLUME] );	
		}
		
		// cout <<  lfo1_out[x] << endl;
		
		if (osc1.start_phase)
		{
			osc1.frequency = fastishP2F(osc1.note + fParameters[dracorex_OSC1_TUNING]
				+ (fParameters[dracorex_OSC1_PITCH_ADSR3] * adsr3_env.level * 12)
				+ (fParameters[dracorex_OSC1_PITCH_ADSR4] * adsr4_env.level * 12)
				+ (12 * lfo1_out[x] * fParameters[dracorex_LFO1_OSC1_PITCH_AMOUNT])) * 0.8175;
			osc1.start_phase = false;
		}
		
		if (osc2.start_phase)
		{
			osc2.frequency = fastishP2F(osc2.note + fParameters[dracorex_OSC2_TUNING]
				+ (fParameters[dracorex_OSC2_PITCH_ADSR3] * adsr3_env.level * 12)
				+ (fParameters[dracorex_OSC2_PITCH_ADSR4] * adsr4_env.level * 12)
				+ (12 * lfo1_out[x] * fParameters[dracorex_LFO1_OSC2_PITCH_AMOUNT])) * 0.8175;
			osc2.start_phase = false;
		}
		
		if (amp_env.state == ENV_STATE_DORMANT && amp2_env.state == ENV_STATE_DORMANT) active = false;
		
		if (fParameters[dracorex_CHORUS_ACTIVE])
		{
			float* chorus_out = _chorus.run(in_left, in_right);
			left_buffer[x] += chorus_out[0];
			right_buffer[x] += chorus_out[1];
		}
		else
		{
			left_buffer[x] += in_left;
			right_buffer[x] += in_right;	
		}
	}

	return 0;	

};
