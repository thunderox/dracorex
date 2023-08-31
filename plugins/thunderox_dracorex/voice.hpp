

#include "oscillator.hpp"
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

class voice
{
	public:

	voice();
	~voice();
	
	float play(float* left_buffer, float* right_buffer,  uint32_t frames);
	
	oscillator osc1;
	oscillator osc2;
};



