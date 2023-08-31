
using namespace std;

#include "wavetable.h"

#define ENV_STATE_ATTACK 1
#define ENV_STATE_DECAY 2
#define ENV_STATE_SUSTAIN 3
#define ENV_STATE_RELEASE 4
#define ENV_STATE_WAIT 5
#define ENV_STATE_DORMANT 6
#define ENV_STATE_KILL 7

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

typedef struct
{
	oscillator osc[4];
	bool active;
	float volume;
	float pan;
	float frequency;

	float buf0_left,buf1_left;
	float f_left, pc_left, q_left;             //filter coefficients
	float bf0_left, bf1_left, bf2_left, bf3_left, bf4_left;  //filter buffers (beware denormals!)
	float t1_left, t2_left;              //temporary buffers
	float selectivity_left, gain1_left, gain2_left, ratio_left, cap_left; 
	float filter_frequency_left,filter_resonance_left;
		
	float buf0_right,buf1_right;
	float f_right, pc_right, q_right;             //filter coefficients
	float bf0_right, bf1_right, bf2_right, bf3_right, bf4_right;  //filter buffers (beware denormals!)
	float t1_right, t2_right;              //temporary buffers
	float selectivity_right, gain1_right, gain2_right, ratio_right, cap_right; 
	float filter_frequency_right,filter_resonance_right;

	// Envelopes

	int env_amp_state;
	float env_amp_level;

	int env_wave_state;
	float env_wave_level;

	int env_adsr3_state;
	float env_adsr3_level;

} voice;

//---------------------------------------------------------------------------------------------------	
	
	
class synth
{
	void init(int);
	void cleanup();
	void play(float* const, float* const, uint32_t);
	void note_on(int, int);
	void note_off(int);
	int get_number_of_voices_playing();
	void set_osc_waves(int, int, int);
};






