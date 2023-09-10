
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

	dracorex_MIDI_IN,
	dracorex_OUT_LEFT,
	dracorex_OUT_RIGHT,
	
	dracorex_OSC1_ACTIVE,
	dracorex_OSC1_WAVE_A,
	dracorex_OSC1_WAVE_B,
	dracorex_OSC1_TUNING,
	dracorex_OSC1_OCTAVE,
	dracorex_OSC1_VOLUME,
	dracorex_AMP_ATTACK,
	dracorex_AMP_DECAY,
	dracorex_AMP_SUSTAIN,
	dracorex_AMP_RELEASE,
	dracorex_WAVE_ATTACK,
	dracorex_WAVE_DECAY,
	dracorex_WAVE_SUSTAIN,
	dracorex_WAVE_RELEASE,
	dracorex_OSC1_PITCH_ADSR3,
	dracorex_OSC1_PITCH_ADSR4,
	
	dracorex_OSC2_ACTIVE,
	dracorex_OSC2_WAVE_A,
	dracorex_OSC2_WAVE_B,
	dracorex_OSC2_TUNING,
	dracorex_OSC2_OCTAVE,
	dracorex_OSC2_VOLUME,
	dracorex_AMP2_ATTACK,
	dracorex_AMP2_DECAY,
	dracorex_AMP2_SUSTAIN,
	dracorex_AMP2_RELEASE,
	dracorex_WAVE2_ATTACK,
	dracorex_WAVE2_DECAY,
	dracorex_WAVE2_SUSTAIN,
	dracorex_WAVE2_RELEASE,
	dracorex_OSC2_PITCH_ADSR3,
	dracorex_OSC2_PITCH_ADSR4,

	dracorex_FILTER_ACTIVE,
	dracorex_CUTOFF,
	dracorex_RESONANCE,
	dracorex_FILTER_ADSR3_AMOUNT,

	dracorex_LFO1_RETRIG,
	dracorex_LFO1_WAVE,
	dracorex_LFO1_SPEED,
	dracorex_LFO1_OSC1_PITCH_AMOUNT,
	dracorex_LFO1_OSC2_PITCH_AMOUNT,
	dracorex_LFO1_OSC1_WAVE_AMOUNT,
	dracorex_LFO1_OSC2_WAVE_AMOUNT,
	dracorex_LFO1_FILTER_CUTOFF_AMOUNT,

	dracorex_LFO2_RETRIG,
	dracorex_LFO2_WAVE,
	dracorex_LFO2_SPEED,
	dracorex_LFO2_OSC1_PITCH_AMOUNT,
	dracorex_LFO2_OSC2_PITCH_AMOUNT,
	dracorex_LFO2_OSC1_WAVE_AMOUNT,
	dracorex_LFO2_OSC2_WAVE_AMOUNT,
	dracorex_LFO2_FILTER_CUTOFF_AMOUNT,
	
	dracorex_CHORUS_ACTIVE,
		
	dracorex_VOLUME,
	dracorex_MASTER_TUNING,

	dracorex_ADSR3_ATTACK,
	dracorex_ADSR3_DECAY,
	dracorex_ADSR3_SUSTAIN,
	dracorex_ADSR3_RELEASE,
	dracorex_ADSR4_ATTACK,
	dracorex_ADSR4_DECAY,
	dracorex_ADSR4_SUSTAIN,
	dracorex_ADSR4_RELEASE,

	kParameterBufferSize,
	kParametertriceratopsTime,        
	kParameterCount
};


#endif // DISTRHO_PLUGIN_INFO_H_INCLUDED
