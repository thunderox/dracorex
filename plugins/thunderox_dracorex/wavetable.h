

#include <stdbool.h>
#include <string>

using namespace std;

#ifndef WAVETABLE
#define WAVETABLE

//---------------------------------------------------------------------------------------------------

typedef struct
{
	int length;
	float *buffer;
} wave;

typedef struct
{
	float frequency;
	float index;
	float increment;
	bool start_phase;
	int wave1_number;
	int wave2_number;
	float wave_mix;
	unsigned int bandlimit_offset;
} oscillator;

//---------------------------------------------------------------------------------------------------

class wavetable
{
	wavetable();
	~wavetable();
	void initialise();
	void cleanup();
	float* get_buffer(int);
	float tick(oscillator);
	void set_oscillator_frequency(oscillator,float);
	void set_oscillator_wave_number(oscillator,int,int);

	void make_wave(int, string, string);
	void set_oscillator_wave_mix(oscillator,float);
	void set_sample_rate(int);
};


#endif
