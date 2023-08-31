

#include <stdbool.h>
#include <string>
#include <vector>

using namespace std;

#ifndef WAVETABLE
#define WAVETABLE

//---------------------------------------------------------------------------------------------------

struct wavetable
{
	string name;
	float* buffer;
	long length;
};

vector <wavetable> wavetables;

typedef struct
{
	float frequency;
	float index;
	float increment;
	bool start_phase;
	string waveA_name;
	string waveB_name;
	float wave_mix;
	unsigned int bandlimit_offset;
} oscillator;

//---------------------------------------------------------------------------------------------------

class synth
{
	synth();
	~synth();
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



