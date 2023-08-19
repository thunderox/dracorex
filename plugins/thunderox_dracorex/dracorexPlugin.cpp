
// ThunderOx dracorex synth - By Nick S. Bailey <tb303@gmx.com>
// DISTRHO Plugin Framework (DPF) Copyright (C) 2012-2019 Filipe Coelho <falktx@falktx.com>

#include "DistrhoPlugin.hpp"
#include <string.h>
#include <string>
#include <iostream>
#include <vector> 
#include <math.h>


using namespace std;

const int max_notes = 12;

START_NAMESPACE_DISTRHO

class dracorexPlugin : public Plugin
{
	public:
		// Parameters
		float fParameters[kParameterCount+3];
		
		// Audio Struct
		struct audio_stereo
		{ 
			float left,right; 
		};
		float srate = getSampleRate();

		int midi_keys[128];
			 
		int current_synth = 0;
		int old_synth = 0;
		
		float* pitch_bend = new float();
		float* channel_after_touch = new float();

			
		// dracorex Audio Buffer
		vector <audio_stereo> audio_buffer; 
					
		dracorexPlugin() : Plugin(kParameterCount, 0, 0)
		{

			srate = getSampleRate();

			audio_buffer.resize(srate*5);
	
			for (int x=0; x<128; x++) { midi_keys[x] = -1; }
			
			// clear all parameters
			std::memset(fParameters, 0, sizeof(float)*kParameterCount);

			// we can know buffer-size right at the start
			fParameters[kParameterBufferSize] = getBufferSize();

		}

		const char* getDescription() const override
		{
			return "dracorex is a polyphonic wavetable synthesizer plugin.";
		}
		
		const char* getHomePage() const override
		{
			return "https://thunderox.com/synths/dracorex";
		}

		const char* getLabel() const
		{
			return "Thunderox dracorex";
		}

		const char* getMaker() const
		{
			return "Nick S. Bailey";
		}

		const char* getLicense() const
		{
			return "ISC";
		}

		//Get the plugin version, in hexadecimal.
    
		uint32_t getVersion() const override
		{
			return d_version(1, 0, 0);
		}

		int64_t getUniqueId() const
		{
			return d_cconst('T', 'O', 'd', 'r');
		}



		void initParameter(uint32_t index, Parameter& parameter) override
		{
			switch (index)
			{

				case dracorex_VOLUME:
					parameter.name   = "volume";
					parameter.symbol = "volume";
					parameter.hints = kParameterIsAutomatable;
					parameter.ranges.min = 0.0f;
					parameter.ranges.max = 1.0f;
					parameter.ranges.def = 0.8f;
					fParameters[dracorex_VOLUME] = parameter.ranges.def;
					break;
				

			}

		}

		/* --------------------------------------------------------------------------------------------------------
		* Internal data 
		Get the current value of a parameter.
 		The host may call this function from any context, including realtime processing.
		*/

		float getParameterValue(uint32_t index) const override
		{
			return fParameters[index];

		}

		/**
  		Change a parameter value.
 		The host may call this function from any context, including realtime processing.
		When a parameter is marked as automable, you must ensure no non-realtime operations are performed.
		@note This function will only be called for parameter inputs.
		*/

		void setParameterValue(uint32_t index, float value) override
		{
			fParameters[index] = value;
			
		}

		void run(const float** inputs, float** outputs, uint32_t frames,
             const MidiEvent* midiEvents, uint32_t midiEventCount) override
		{

			// if (!fParameters[dracorex_DSP_RUN]) return; // Playing sound during preset load is a bad idea.	
			
			float* out_left = outputs[0];
			float* out_right = outputs[1];
			
			memset( out_left, 0, sizeof(double)*(frames*0.5) );
			memset( out_right, 0, sizeof(double)*(frames*0.5) );
		

		}


		//----------------------------------------------
		 				
		void initProgramName(uint32_t index, String& program_name) override
		{
		}
	
		
		//----------------------------------------------

	private:

		bool preset_loading = false;
		int bpm;
		uint32_t buffer_length;
};

Plugin* createPlugin()
{

	dracorexPlugin* dracorex = new dracorexPlugin();

	return dracorex;
}      

END_NAMESPACE_DISTRHO





