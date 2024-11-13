
#include "sine_osc.hpp"



//---------------------------------------------------------------------------------------------------

sine_osc::sine_osc()
{
	start_phase = true;
	frequency = 1;
	index = 0;
	increment = 1;
	bandlimit_offset = 0;
	active = false;
	noise = 0;
	length = 17720;

	sample_rate = 44100;
	
	sine_generator sg;
}

//---------------------------------------------------------------------------------------------------

sine_osc::~sine_osc()
{

}


//-----------------------------------------------------------
// Play wavetable 

float sine_osc::tick()
{
	if (frequency < 0.01) frequency = 0.01;
	
	sg.set_frequency(frequency * 2 * 3.141592653589793 / 8000);
	double test[16];
	sg.generate(1,&test[0]);

	return test[0];

}





	
	
	
