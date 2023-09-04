
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
				if (wave_env.level <= 0)
				{
					wave2_env.level = 0;
					wave2_env.state = ENV_STATE_DORMANT;
				}
			}
			break;
		}
		
			
		//float env_amp_level_db = (amp_env.level * amp_env.level * amp_env.level) * master_volume; 

		osc1.wave_mix = wave_env.level;
		osc2.wave_mix = wave2_env.level;
				
		float osc1_out;
		float osc2_out;
		
		if (fParameters[dracorex_OSC1_ACTIVE]) osc1_out = osc1.tick(); else osc1_out = 0;
		if (fParameters[dracorex_OSC2_ACTIVE]) osc2_out = osc2.tick(); else osc2_out = 0;		

	
		left_buffer[x] += osc1_out * (amp_env.level * fParameters[dracorex_VOLUME] );
		right_buffer[x] += osc1_out * (amp_env.level * fParameters[dracorex_VOLUME] );
		left_buffer[x] += osc2_out * (amp2_env.level * fParameters[dracorex_VOLUME] );
		right_buffer[x] += osc2_out * (amp2_env.level * fParameters[dracorex_VOLUME] );
		
		if (amp_env.state == ENV_STATE_DORMANT && amp2_env.state == ENV_STATE_DORMANT) active = false;
		
	}

	return 0;	

};
