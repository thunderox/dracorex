
#include <stdlib.h>

#include "uris.h"
#include "lv2/lv2plug.in/ns/lv2core/lv2.h"
#include "lv2/lv2plug.in/ns/ext/atom/forge.h"


typedef enum {
	dracorex_MIDI_IN   = 0,
	dracorex_OUT_LEFT  = 1,
	dracorex_OUT_RIGHT = 2,
	dracorex_AMP_ATTACK = 3,
	dracorex_AMP_DECAY = 4,
	dracorex_AMP_SUSTAIN = 5,
	dracorex_AMP_RELEASE = 6,
	dracorex_WAVE_ATTACK = 7,
	dracorex_WAVE_DECAY = 8,
	dracorex_WAVE_SUSTAIN = 9,
	dracorex_WAVE_RELEASE = 10,
	dracorex_OSC1_ACTIVE = 11,
	dracorex_OSC2_ACTIVE = 12,
	dracorex_OSC1_WAVE_A = 13,
	dracorex_OSC1_WAVE_B = 14,
	dracorex_OSC2_WAVE_A = 15,
	dracorex_OSC2_WAVE_B = 16,
	dracorex_OSC1_TUNING = 17,
	dracorex_OSC2_TUNING = 18,
	dracorex_VOLUME = 19,

	dracorex_CUTOFF = 20,
	dracorex_RESONANCE = 21,
	dracorex_FILTER_ACTIVE = 22,

	dracorex_CHORUS_ACTIVE = 23,

	dracorex_LFO1_WAVE = 24,
	dracorex_LFO1_SPEED = 25,
	dracorex_LFO1_RETRIG = 26,
	dracorex_LFO1_OSC1_AMOUNT = 27,
	dracorex_LFO1_OSC2_AMOUNT = 28,
	dracorex_LFO1_OSC1_WAVE_AMOUNT = 29,
	dracorex_LFO1_OSC2_WAVE_AMOUNT = 30,
	dracorex_LFO1_FILTER_AMOUNT = 31,

	dracorex_LFO2_WAVE = 32,
	dracorex_LFO2_SPEED = 33,
	dracorex_LFO2_RETRIG = 34,
	dracorex_LFO2_OSC1_AMOUNT = 35,
	dracorex_LFO2_OSC2_AMOUNT = 36,
	dracorex_LFO2_OSC1_WAVE_AMOUNT = 37,
	dracorex_LFO2_OSC2_WAVE_AMOUNT = 38,
	dracorex_LFO2_FILTER_AMOUNT = 39,
	dracorex_ADSR3_ATTACK = 40,
	dracorex_ADSR3_DECAY = 41,
	dracorex_ADSR3_SUSTAIN = 42,
	dracorex_ADSR3_RELEASE = 43,
	dracorex_OSC1_PITCH_ADSR2 = 44,
	dracorex_OSC1_PITCH_ADSR3 = 45,
	dracorex_OSC2_PITCH_ADSR2 = 46,
	dracorex_OSC2_PITCH_ADSR3 = 47

} PortIndex;

typedef struct {
	float* out_left;
	float* out_right;
	float* wave_one;

	float* amp_attack;
	float* amp_decay;
	float* amp_sustain;
	float* amp_release;

	float* wave_attack;
	float* wave_decay;
	float* wave_sustain;
	float* wave_release;

	float* adsr3_attack;
	float* adsr3_decay;
	float* adsr3_sustain;
	float* adsr3_release;

	float* osc1_wave_a;
	float* osc1_wave_b;

	float* lfo_speed;
	float* lfo_osc1_amount;
	float* lfo_osc2_amount;	

	float* params[64];

	dracorexURIs uris;
	LV2_Atom_Forge forge;
	LV2_URID_Map* map;
	char *bundle_path;

	LV2_Atom_Sequence* midi_in;

} dracorex;
