

#include "oscillator.hpp"
#include "chorus.hpp"
#include "DistrhoPlugin.hpp"
#include <string>
#include <iostream>

using namespace std;

#define ENV_STATE_ATTACK 1
#define ENV_STATE_DECAY 2
#define ENV_STATE_SUSTAIN 3
#define ENV_STATE_RELEASE 4
#define ENV_STATE_WAIT 5
#define ENV_STATE_DORMANT 6
#define ENV_STATE_KILL 7


struct wavetable
{
	string name;
	float* buffer;
	long length;
};

struct adsr
{
	float attack;
	float decay;
	float sustain;
	float release;
	float level;
	int state;
};

class voice
{
	public:

	voice();
	~voice();
	
	bool active;
	
	float play(float*, float*,  uint32_t);
	
	oscillator osc1;
	oscillator osc2;
	float* lfo1_out;
	float* lfo2_out;
	chorus _chorus;
	
	adsr amp_env;
	adsr wave_env;
	adsr amp2_env;
	adsr wave2_env;
	adsr adsr3_env;
	adsr adsr4_env;	
	
	float buf0_left,buf1_left;
	float f_left, pc_left, q_left;             //filter coefficients
	float bf0_left, bf1_left, bf2_left, bf3_left, bf4_left;  //filter buffers (beware denormals!)
	float t1_left, t2_left;              //temporary buffers
	float selectivity_left, gain1_left, gain2_left, ratio_left, cap_left; 
	
	float buf0_right,buf1_right;
	float f_right, pc_right, q_right;             //filter coefficients
	float bf0_right, bf1_right, bf2_right, bf3_right, bf4_right;  //filter buffers (beware denormals!)
	float t1_right, t2_right;              //temporary buffers
	float selectivity_right, gain1_right, gain2_right, ratio_right, cap_right; 
	
	// Parameters
	float* fParameters;
};



