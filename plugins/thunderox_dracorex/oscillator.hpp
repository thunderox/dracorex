
#include <stdbool.h>
#include <string>

#include <iostream>
#include <vector>

using namespace std;

//---------------------------------------------------------------------------------------------------




//---------------------------------------------------------------------------------------------------

class oscillator
{
	public:
	
	oscillator();
	~oscillator();
	float tick();
	
	float frequency;
	int length;
	int note;
	float index;
	float increment;
	bool start_phase;
	float* wave_a;
	float* wave_b;
	float wave_mix;
	unsigned int bandlimit_offset;
	
	float tuning;
	int sample_rate;
};
