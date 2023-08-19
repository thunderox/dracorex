
#ifndef DISTRHO_PLUGIN_INFO_H_INCLUDED
#define DISTRHO_PLUGIN_INFO_H_INCLUDED

#define DISTRHO_PLUGIN_NAME "thunderox_dracorex"
#define DISTRHO_PLUGIN_NUM_INPUTS 0
#define DISTRHO_PLUGIN_NUM_OUTPUTS 2
#define DISTRHO_PLUGIN_URI "http://thunderox.com/thunderox_dracorex"
#define DISTRHO_PLUGIN_HAS_UI 1
#define DISTRHO_UI_USE_CAIRO 1
#define DISTRHO_PLUGIN_IS_RT_SAFE 1
#define DISTRHO_PLUGIN_IS_SYNTH 1
#define DISTRHO_PLUGIN_WANT_PROGRAMS 1
#define DISTRHO_PLUGIN_WANT_TIMEPOS 1

#define DISTRHO_UI_URI DISTRHO_PLUGIN_URI "#UI"

enum Parameters {

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
	dracorex_OSC2_PITCH_ADSR3 = 47,
	
	kParameterBufferSize,
	kParametertriceratopsTime,        
	kParameterCount
};


#endif // DISTRHO_PLUGIN_INFO_H_INCLUDED
