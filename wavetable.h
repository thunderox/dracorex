

#include <stdbool.h>

#ifndef WAVETABLE
#define WAVETABLE

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

	void wavetable_initialise();
	void wavetable_cleanup();
	float* wavetable_get_buffer(int);
	float wavetable_tick(oscillator*);
	void wavetable_set_oscillator_frequency(oscillator*,float);
	void wavetable_set_oscillator_wave_number(oscillator*,int,int);

	void wavetable_make_wave(int, const char*, const char*);
	void wavetable_set_oscillator_wave_mix(oscillator*,float);
	void wavetable_set_sample_rate(int);

#endif
