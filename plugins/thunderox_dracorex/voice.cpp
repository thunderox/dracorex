
#include "voice.hpp"

//----------------------------------------------------------------------------------------------------------------

voice::voice()
{
	amp_env.state = ENV_STATE_DORMANT;
	amp_env.level = 0;
	wave_env.state = ENV_STATE_DORMANT;
	wave_env.level = 0;
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

	for (uint32_t x=0; x<frames; x++)
	{
	
		//--------- ADSR AMP
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
					active = false; 	
				}
			}
			break;
		}
					
		//float env_amp_level_db = (amp_env.level * amp_env.level * amp_env.level) * master_volume; 

	
		float osc1_out = osc1.tick();
	
		left_buffer[x] += osc1_out * (amp_env.level * fParameters[dracorex_VOLUME] );
		right_buffer[x] += osc1_out * (amp_env.level * fParameters[dracorex_VOLUME] );
	}

	return 0;	

};
