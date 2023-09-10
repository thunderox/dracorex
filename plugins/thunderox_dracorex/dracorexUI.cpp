

#include "DistrhoPluginInfo.h"
#include "DistrhoUI.hpp"
#include "Window.hpp"
#include <iostream>
#include <sstream> 
#include <fstream>
#include <dirent.h>
#include <stdlib.h>
#include <sys/stat.h>

using namespace std;

#include "../../delirium_ui/delirium_ui.hpp"

START_NAMESPACE_DISTRHO

class dracorexUI : public UI
{
	public:

		//------------------------------------------------------------------------------------------------------

		dracorexUI() : UI(640, 500)
		{

			memset(fParameters, 0, sizeof(float)*kParameterCount);
			memset(fParameters_widget_number, -1, sizeof(int)*kParameterCount);

			GUI = Delirium_UI_Init(static_cast<float>(getWidth()),static_cast<float>(getHeight()), 26,20);
			
			// OSCILATOR PANEL
			
			float panelX = 0.5;
			float panelY = 0.5;

			GUI->group_visible[0] = true;
			
			//------------------------------------------------------------------------------------------------------------------------------------------
			
			// SET UP GUI WIDGETS -  -----------------------------------------------------------------------
			
			
			int osc_panel = Delirium_UI_Create_Widget(GUI, deliriumUI_Panel, 0, panelX, panelY, 13,14, "OSCILLATORS", -1);
			
			Delirium_UI_Widget_Set_Group_And_Member(GUI, osc_panel, "global", "");

						
			// OSCILATOR NAVIGATION ONE / TWO -----------------------------------------------------------------------
			
			int oscillators_group = Delirium_UI_Group_Create(GUI, "oscillators");
			Delirium_UI_Group_Add_Member(GUI, "oscillators", "osc1");
			Delirium_UI_Group_Add_Member(GUI, "oscillators", "osc2");
			
			int nav_oscillators = Delirium_UI_Create_Widget(GUI,  deliriumUI_Tabbed_Navigator,
				0, panelX , panelY + 0.25, 12,0.6,"",-1);			
			Delirium_UI_Group_Add_Navigator_Tab(GUI, nav_oscillators, "OSC-1", "oscillators", "osc1");
			Delirium_UI_Group_Add_Navigator_Tab(GUI, nav_oscillators, "OSC-2", "oscillators", "osc2");	
			
			Delirium_UI_Group_Set_Visible_member(GUI, "oscillators", "osc1");
			
			Delirium_UI_Widget_Set_Group_And_Member(GUI, nav_oscillators, "global", "");
					

			//------- OSCILLATOR ONE - ACTIVE -----------------------------------------------------------------------
			int widget_osc1_active = Delirium_UI_Create_Widget(GUI, deliriumUI_Switch, 0, panelX + 0.5, panelY + 1.25, 2, 2.2, "OSC-1", dracorex_OSC1_ACTIVE);

			Delirium_UI_Widget_Set_Min_Max(GUI, widget_osc1_active, 0,1);
			Delirium_UI_Widget_Set_Value(GUI, widget_osc1_active, 1);
			Delirium_UI_Widget_Set_Group_And_Member(GUI, widget_osc1_active, "oscillators", "osc1");			
			fParameters_widget_number[dracorex_OSC1_ACTIVE] = widget_osc1_active; 
			
			//--------- OSCILLATOR ONE - VOLUME -----------------------------------------------------------------------------------
			
			int widget_osc1_volume = Delirium_UI_Create_Widget(GUI, deliriumUI_Knob, 0, panelX + 4.5,panelY + 1.25, 2, 2.5, "VOL", dracorex_OSC1_VOLUME);
			Delirium_UI_Widget_Set_Min_Max(GUI, widget_osc1_volume, 0,1);
			Delirium_UI_Widget_Set_Value(GUI, widget_osc1_volume, 0.5);
			Delirium_UI_Widget_Set_Group_And_Member(GUI, widget_osc1_volume, "oscillators", "osc1");
			fParameters_widget_number[dracorex_OSC1_VOLUME] = widget_osc1_volume; 
			
			//------- OSCILLATOR ONE - DETUNE -----------------------------------------------------------------------

			int widget_osc1_detune = Delirium_UI_Create_Widget(GUI, deliriumUI_Fader, 0, panelX + 1, panelY + 4, 1, 5.25, "DETUNE", dracorex_OSC1_TUNING);
			Delirium_UI_Widget_Set_Min_Max(GUI, widget_osc1_detune, 7,-7);
			Delirium_UI_Widget_Set_Value(GUI, widget_osc1_detune, 0);
			Delirium_UI_Widget_Set_Group_And_Member(GUI, widget_osc1_detune, "oscillators", "osc1");
			fParameters_widget_number[dracorex_OSC1_TUNING] = widget_osc1_detune;
			
			//------- Oscilator One - ADSR2 to OSC 1 Pitch  -----------------------------------------------------------------------
			
			int widget_osc1_adsr3_amount = Delirium_UI_Create_Widget(GUI, deliriumUI_Fader, 0, panelX + 3, panelY + 4, 1, 5.25, "ADSR3", dracorex_OSC1_PITCH_ADSR3);
			Delirium_UI_Widget_Set_Min_Max(GUI, widget_osc1_adsr3_amount, 1,0);
			Delirium_UI_Widget_Set_Value(GUI, widget_osc1_adsr3_amount, 0);
			Delirium_UI_Widget_Set_Group_And_Member(GUI, widget_osc1_adsr3_amount, "oscillators", "osc1");
			fParameters_widget_number[dracorex_OSC1_PITCH_ADSR3] = widget_osc1_adsr3_amount; 
			
			//------- Oscilator One - ADSR3 to OSC 1 Pitch  -----------------------------------------------------------------------
			
			int widget_osc1_adsr4_amount = Delirium_UI_Create_Widget(GUI, deliriumUI_Fader, 0, panelX + 5, panelY + 4, 1, 5.25, "ADSR4", dracorex_OSC1_PITCH_ADSR4);
			Delirium_UI_Widget_Set_Min_Max(GUI, widget_osc1_adsr4_amount, 1,0);
			Delirium_UI_Widget_Set_Value(GUI, widget_osc1_adsr4_amount, 0);
			Delirium_UI_Widget_Set_Group_And_Member(GUI, widget_osc1_adsr4_amount, "oscillators", "osc1");
			fParameters_widget_number[dracorex_OSC1_PITCH_ADSR4] = widget_osc1_adsr4_amount; 
			

			//------- Oscilator One - ADSR1 - OSC1 AMP ENVELOPE -----------------------------------------------------------------------

			int widget_amp_env = Delirium_UI_Create_Widget(GUI, deliriumUI_ADSR, 0, panelX + 1, panelY + 11, 5, 3,"ADSR1 - AMP",dracorex_AMP_ATTACK); 
			Delirium_UI_Widget_Set_Group_And_Member(GUI, widget_amp_env, "oscillators", "osc1");	
			fParameters_widget_number[dracorex_AMP_ATTACK] = widget_amp_env; 
			fParameters_widget_number[dracorex_AMP_ATTACK+1] = widget_amp_env; 
			fParameters_widget_number[dracorex_AMP_ATTACK+2] = widget_amp_env; 
			fParameters_widget_number[dracorex_AMP_ATTACK+3] = widget_amp_env; 
			
			//------- Oscilator One - ADSR2 - OSC1 WAVE ENVELOPE -----------------------------------------------------------------------
			
			int widget_wave_env = Delirium_UI_Create_Widget(GUI, deliriumUI_ADSR, 0, panelX + 7, panelY + 11, 5, 3,"ADSR2 - WAVE",dracorex_WAVE_ATTACK); 
			Delirium_UI_Widget_Set_Group_And_Member(GUI, widget_wave_env, "oscillators", "osc1");	
			fParameters_widget_number[dracorex_WAVE_ATTACK] = widget_wave_env; 
			fParameters_widget_number[dracorex_WAVE_ATTACK+1] = widget_wave_env; 
			fParameters_widget_number[dracorex_WAVE_ATTACK+2] = widget_wave_env; 
			fParameters_widget_number[dracorex_WAVE_ATTACK+3] = widget_wave_env; 
			
			//------- Oscilator One - OSC1 WAVE A SELECTOR
			
			int dracorex_osc1_waveA = Delirium_UI_Create_Widget(GUI, deliriumUI_Selector, 0, panelX +8, panelY + 1.25, 4, 3,"WAVE A", dracorex_OSC1_WAVE_A);
			Delirium_UI_Widget_Set_Selector_Type(GUI, dracorex_osc1_waveA, WAVE_MODE_LFO);
			Delirium_UI_Widget_Set_Group_And_Member(GUI, dracorex_osc1_waveA, "oscillators", "osc1");
			fParameters_widget_number[dracorex_OSC1_WAVE_A] = dracorex_osc1_waveA;
			
			//------- Oscilator One - OSC1 WAVE B SELECTOR		
	
			int dracorex_osc1_waveB = Delirium_UI_Create_Widget(GUI, deliriumUI_Selector, 0, panelX +8, panelY + 5, 4, 3,"WAVE B", dracorex_OSC1_WAVE_B);
			Delirium_UI_Widget_Set_Selector_Type(GUI, dracorex_osc1_waveB, WAVE_MODE_LFO);
			Delirium_UI_Widget_Set_Group_And_Member(GUI, dracorex_osc1_waveB, "oscillators", "osc1");
			fParameters_widget_number[dracorex_OSC1_WAVE_B] = dracorex_osc1_waveB;			
			
			
			//--------------------------------------------------------------------------------------------------------------------------------
			// OSCILATOR TWO ------------------------------------------------------------------------------------------------------------------
		

			//------- OSCILLATOR TWO - ACTIVE -----------------------------------------------------------------------
			int widget_osc2_active = Delirium_UI_Create_Widget(GUI, deliriumUI_Switch, 0, panelX + 0.5, panelY + 1.25, 2, 2.2, "OSC-2", dracorex_OSC2_ACTIVE);

			Delirium_UI_Widget_Set_Min_Max(GUI, widget_osc2_active, 0,1);
			Delirium_UI_Widget_Set_Value(GUI, widget_osc2_active, 1);
			Delirium_UI_Widget_Set_Group_And_Member(GUI, widget_osc2_active, "oscillators", "osc2");			
			fParameters_widget_number[dracorex_OSC2_ACTIVE] = widget_osc2_active; 
			
			//--------- OSCILLATOR ONE - VOLUME -----------------------------------------------------------------------------------
			
			int widget_osc2_volume = Delirium_UI_Create_Widget(GUI, deliriumUI_Knob, 0, panelX + 4.5,panelY + 1.25, 2, 2.5, "VOL", dracorex_OSC2_VOLUME);
			Delirium_UI_Widget_Set_Min_Max(GUI, widget_osc2_volume, 0,1);
			Delirium_UI_Widget_Set_Value(GUI, widget_osc2_volume, 0.5);
			Delirium_UI_Widget_Set_Group_And_Member(GUI, widget_osc2_volume, "oscillators", "osc2");
			fParameters_widget_number[dracorex_OSC2_VOLUME] = widget_osc2_volume; 
			
			//------- OSCILLATOR TWO - DETUNE -----------------------------------------------------------------------

			int widget_osc2_detune = Delirium_UI_Create_Widget(GUI, deliriumUI_Fader, 0, panelX + 1, panelY + 4, 1, 5.25, "DETUNE", dracorex_OSC2_TUNING);
			Delirium_UI_Widget_Set_Min_Max(GUI, widget_osc2_detune, 7,-7);
			Delirium_UI_Widget_Set_Value(GUI, widget_osc2_detune, 0);
			Delirium_UI_Widget_Set_Group_And_Member(GUI, widget_osc2_detune, "oscillators", "osc2");
			fParameters_widget_number[dracorex_OSC2_TUNING] = widget_osc2_detune;
			
			//------- Oscilator TWO - ADSR2 to OSC 1 Pitch  -----------------------------------------------------------------------
			
			int widget_osc2_adsr3_amount = Delirium_UI_Create_Widget(GUI, deliriumUI_Fader, 0, panelX + 3, panelY + 4, 1, 5.25, "ADSR3", dracorex_OSC2_PITCH_ADSR3);
			Delirium_UI_Widget_Set_Min_Max(GUI, widget_osc2_adsr3_amount, 1,0);
			Delirium_UI_Widget_Set_Value(GUI, widget_osc2_adsr3_amount, 0);
			Delirium_UI_Widget_Set_Group_And_Member(GUI, widget_osc2_adsr3_amount, "oscillators", "osc2");
			fParameters_widget_number[dracorex_OSC2_PITCH_ADSR3] = widget_osc2_adsr3_amount; 
			
			//------- Oscilator TWO - ADSR3 to OSC 1 Pitch  -----------------------------------------------------------------------
			
			int widget_osc2_adsr4_amount = Delirium_UI_Create_Widget(GUI, deliriumUI_Fader, 0, panelX + 5, panelY + 4, 1, 5.25, "ADSR4", dracorex_OSC2_PITCH_ADSR4);
			Delirium_UI_Widget_Set_Min_Max(GUI, widget_osc2_adsr4_amount, 1,0);
			Delirium_UI_Widget_Set_Value(GUI, widget_osc2_adsr4_amount, 0);
			Delirium_UI_Widget_Set_Group_And_Member(GUI, widget_osc2_adsr4_amount, "oscillators", "osc2");
			fParameters_widget_number[dracorex_OSC2_PITCH_ADSR4] = widget_osc2_adsr4_amount; 
			

			//------- Oscilator TWO - ADSR1 - OSC2 AMP ENVELOPE -----------------------------------------------------------------------

			int widget_amp2_env = Delirium_UI_Create_Widget(GUI, deliriumUI_ADSR, 0, panelX + 1,  panelY + 11, 5, 3,"ADSR2 - AMP",dracorex_AMP2_ATTACK); 
			Delirium_UI_Widget_Set_Group_And_Member(GUI, widget_amp2_env, "oscillators", "osc2");	
			fParameters_widget_number[dracorex_AMP2_ATTACK] = widget_amp2_env; 
			fParameters_widget_number[dracorex_AMP2_ATTACK+1] = widget_amp2_env; 
			fParameters_widget_number[dracorex_AMP2_ATTACK+2] = widget_amp2_env; 
			fParameters_widget_number[dracorex_AMP2_ATTACK+3] = widget_amp2_env; 
			
			//------- Oscilator TWO - ADSR2 - OSC2 WAVE ENVELOPE -----------------------------------------------------------------------
			
			int widget_wave2_env = Delirium_UI_Create_Widget(GUI, deliriumUI_ADSR, 0, panelX + 7,  panelY + 11, 5, 3,"ADSR2 - WAVE",dracorex_WAVE2_ATTACK); 
			Delirium_UI_Widget_Set_Group_And_Member(GUI, widget_wave2_env, "oscillators", "osc2");	
			fParameters_widget_number[dracorex_WAVE2_ATTACK] = widget_wave2_env; 
			fParameters_widget_number[dracorex_WAVE2_ATTACK+1] = widget_wave2_env; 
			fParameters_widget_number[dracorex_WAVE2_ATTACK+2] = widget_wave2_env; 
			fParameters_widget_number[dracorex_WAVE2_ATTACK+3] = widget_wave2_env; 
			
			//------- Oscilator TWO - OSC2 WAVE A SELECTOR
			
			int dracorex_osc2_waveA = Delirium_UI_Create_Widget(GUI, deliriumUI_Selector, 0, panelX +8, panelY + 1.25, 4, 3,"WAVE A", dracorex_OSC2_WAVE_A);
			Delirium_UI_Widget_Set_Selector_Type(GUI, dracorex_osc2_waveA, WAVE_MODE_LFO);
			Delirium_UI_Widget_Set_Group_And_Member(GUI, dracorex_osc2_waveA, "oscillators", "osc2");
			fParameters_widget_number[dracorex_OSC2_WAVE_A] = dracorex_osc2_waveA;
			
			//------- Oscilator TWO - OSC2 WAVE B SELECTOR		
	
			int dracorex_osc2_waveB = Delirium_UI_Create_Widget(GUI, deliriumUI_Selector, 0, panelX +8, panelY + 5, 4, 3,"WAVE B", dracorex_OSC2_WAVE_B);
			Delirium_UI_Widget_Set_Selector_Type(GUI, dracorex_osc2_waveB, WAVE_MODE_LFO);
			Delirium_UI_Widget_Set_Group_And_Member(GUI, dracorex_osc2_waveB, "oscillators", "osc2");
			fParameters_widget_number[dracorex_OSC2_WAVE_B] = dracorex_osc2_waveB;			
			
			
			

			
			//--------- PANEL MATRIX FOR LFO / FILTER FX ---------------------------------------------------------
			
			int matrix_panel = Delirium_UI_Create_Widget(GUI, deliriumUI_Panel, 0, panelX+13, panelY, 12,8, "MATRIX", -1);
			Delirium_UI_Widget_Set_Group_And_Member(GUI, matrix_panel, "global", "");
			
			//------ MATRIX NAVIGATION LFO / FILTER / FX ----------------------------------------------------------
			
			int matrix_group = Delirium_UI_Group_Create(GUI, "matrix");
			Delirium_UI_Group_Add_Member(GUI, "matrix", "lfo1");
			Delirium_UI_Group_Add_Member(GUI, "matrix", "lfo2");
			Delirium_UI_Group_Add_Member(GUI, "matrix", "filter");
			Delirium_UI_Group_Add_Member(GUI, "matrix", "fx");
			
			int nav_matrix = Delirium_UI_Create_Widget(GUI,  deliriumUI_Tabbed_Navigator,
				0, panelX + 13, panelY + 0.25, 12,0.6,"",-1);	
						
			Delirium_UI_Group_Add_Navigator_Tab(GUI, nav_matrix, "LFO1", "matrix", "lfo1");
			Delirium_UI_Group_Add_Navigator_Tab(GUI, nav_matrix, "LFO2", "matrix", "lfo2");
			Delirium_UI_Group_Add_Navigator_Tab(GUI, nav_matrix, "FILTER", "matrix", "filter");	
			Delirium_UI_Group_Add_Navigator_Tab(GUI, nav_matrix, "FX", "matrix", "fx");	
			
			Delirium_UI_Group_Set_Visible_member(GUI, "matrix", "lfo1");
			
			Delirium_UI_Widget_Set_Group_And_Member(GUI, nav_matrix, "global", "");
			
			//--------- LFO 1 --------------------------------------------------------------------------------------
			
			int widget_lfo1_retrig = Delirium_UI_Create_Widget(GUI, deliriumUI_Switch, 0, panelX + 13.5, panelY + 1.25, 2, 2, "RETRIG", 0);
			Delirium_UI_Widget_Set_Min_Max(GUI, widget_lfo1_retrig, 0,1);
			Delirium_UI_Widget_Set_Value(GUI, widget_lfo1_retrig, 0);
			Delirium_UI_Widget_Set_Group_And_Member(GUI, widget_lfo1_retrig, "matrix", "lfo1");
			fParameters_widget_number[0] = widget_lfo1_retrig; 
			
			//--------- LFO ONE SPEED -----------------------------------------------------------------------------------
			
			int widget_lfo1_speed = Delirium_UI_Create_Widget(GUI, deliriumUI_Knob, 0, panelX + 13.5, panelY + 4, 2, 2.5, "BPM", dracorex_LFO1_SPEED);
			Delirium_UI_Widget_Set_Min_Max(GUI, widget_lfo1_speed, 1,600);
			Delirium_UI_Widget_Set_Value(GUI, widget_lfo1_speed, 120);
			Delirium_UI_Widget_Set_Default_Value(GUI, widget_lfo1_speed, 120);
			Delirium_UI_Widget_Set_Increment(GUI, widget_lfo1_speed, 1);
			Delirium_UI_Widget_Set_Integer(GUI, widget_lfo1_speed, true);
			Delirium_UI_Widget_Set_Group_And_Member(GUI, widget_lfo1_speed, "matrix", "lfo1");
			fParameters_widget_number[dracorex_LFO1_SPEED] = widget_lfo1_speed; 
			
			//--------- LFO ONE WAVE -----------------------------------------------------------------------------------
			
			int dracorex_lfo1_wave = Delirium_UI_Create_Widget(GUI, deliriumUI_Selector, 0, panelX +16, panelY + 1.25, 4, 3,"WAVE", dracorex_LFO1_WAVE);
			Delirium_UI_Widget_Set_Selector_Type(GUI, dracorex_lfo1_wave, WAVE_MODE_LFO);
			Delirium_UI_Widget_Set_Group_And_Member(GUI, dracorex_lfo1_wave, "matrix", "lfo1");
			fParameters_widget_number[dracorex_LFO1_WAVE] = dracorex_lfo1_wave;
			
			//------- LFO ONE OSC1 PITCH -----------------------------------------------------------------------------
			
			int widget_lfo1_osc1_pitch_amount = Delirium_UI_Create_Widget(GUI, deliriumUI_Fader, 0, panelX + 21, panelY + 1.25, 1, 5.25, "OSC1-P", dracorex_LFO1_OSC1_PITCH_AMOUNT);
			Delirium_UI_Widget_Set_Min_Max(GUI, widget_lfo1_osc1_pitch_amount, 1,0);
			Delirium_UI_Widget_Set_Value(GUI, widget_lfo1_osc1_pitch_amount, 0);
			Delirium_UI_Widget_Set_Group_And_Member(GUI, widget_lfo1_osc1_pitch_amount, "matrix", "lfo1");
			fParameters_widget_number[dracorex_LFO1_OSC1_PITCH_AMOUNT] = widget_lfo1_osc1_pitch_amount; 
			
			//------- LFO ONE OSC2 PITCH -----------------------------------------------------------------------------
			
			int widget_lfo1_osc2_pitch_amount = Delirium_UI_Create_Widget(GUI, deliriumUI_Fader, 0, panelX + 23.25, panelY + 1.25, 1, 5.25, "OSC2-P", dracorex_LFO1_OSC2_PITCH_AMOUNT);
			Delirium_UI_Widget_Set_Min_Max(GUI, widget_lfo1_osc2_pitch_amount, 1,0);
			Delirium_UI_Widget_Set_Value(GUI, widget_lfo1_osc2_pitch_amount, 0);
			Delirium_UI_Widget_Set_Group_And_Member(GUI, widget_lfo1_osc2_pitch_amount, "matrix", "lfo1");
			fParameters_widget_number[dracorex_LFO1_OSC2_PITCH_AMOUNT] = widget_lfo1_osc2_pitch_amount; 
			

			
			//--------- LFO 2 --------------------------------------------------------------------------------------
			
			int widget_lfo2_retrig = Delirium_UI_Create_Widget(GUI, deliriumUI_Switch, 0, panelX + 13.5, panelY + 1.25, 2, 2, "RETRIG", 0);
			Delirium_UI_Widget_Set_Min_Max(GUI, widget_lfo2_retrig, 0,1);
			Delirium_UI_Widget_Set_Value(GUI, widget_lfo2_retrig, 0);
			Delirium_UI_Widget_Set_Group_And_Member(GUI, widget_lfo2_retrig, "matrix", "lfo2");
			fParameters_widget_number[0] = widget_lfo2_retrig; 
			
			//--------- LFO ONE SPEED -----------------------------------------------------------------------------------
			
			int widget_lfo2_speed = Delirium_UI_Create_Widget(GUI, deliriumUI_Knob, 0, panelX + 13.5, panelY + 4, 2, 2.5, "BPM", dracorex_LFO2_SPEED);
			Delirium_UI_Widget_Set_Min_Max(GUI, widget_lfo2_speed, 1,600);
			Delirium_UI_Widget_Set_Value(GUI, widget_lfo2_speed, 120);
			Delirium_UI_Widget_Set_Default_Value(GUI, widget_lfo2_speed, 120);
			Delirium_UI_Widget_Set_Increment(GUI, widget_lfo2_speed, 1);
			Delirium_UI_Widget_Set_Integer(GUI, widget_lfo2_speed, true);
			Delirium_UI_Widget_Set_Group_And_Member(GUI, widget_lfo2_speed, "matrix", "lfo2");
			fParameters_widget_number[dracorex_LFO2_SPEED] = widget_lfo2_speed; 
			
			//--------- LFO ONE WAVE -----------------------------------------------------------------------------------
			
			int dracorex_lfo2_wave = Delirium_UI_Create_Widget(GUI, deliriumUI_Selector, 0, panelX +16, panelY + 1.25, 4, 3,"WAVE", dracorex_LFO2_WAVE);
			Delirium_UI_Widget_Set_Selector_Type(GUI, dracorex_lfo2_wave, WAVE_MODE_LFO);
			Delirium_UI_Widget_Set_Group_And_Member(GUI, dracorex_lfo2_wave, "matrix", "lfo2");
			fParameters_widget_number[dracorex_LFO2_WAVE] = dracorex_lfo2_wave;
			
			//------- LFO ONE OSC1 PITCH -----------------------------------------------------------------------------
			
			int widget_lfo2_osc1_pitch_amount = Delirium_UI_Create_Widget(GUI, deliriumUI_Fader, 0, panelX + 21, panelY + 1.25, 1, 5.25, "OSC1-P", dracorex_LFO2_OSC1_PITCH_AMOUNT);
			Delirium_UI_Widget_Set_Min_Max(GUI, widget_lfo2_osc1_pitch_amount, 1,0);
			Delirium_UI_Widget_Set_Value(GUI, widget_lfo2_osc1_pitch_amount, 0);
			Delirium_UI_Widget_Set_Group_And_Member(GUI, widget_lfo2_osc1_pitch_amount, "matrix", "lfo2");
			fParameters_widget_number[dracorex_LFO2_OSC1_PITCH_AMOUNT] = widget_lfo2_osc1_pitch_amount; 
			
			//------- LFO ONE OSC2 PITCH -----------------------------------------------------------------------------
			
			int widget_lfo2_osc2_pitch_amount = Delirium_UI_Create_Widget(GUI, deliriumUI_Fader, 0, panelX + 23.25, panelY + 1.25, 1, 5.25, "OSC2-P", dracorex_LFO2_OSC2_PITCH_AMOUNT);
			Delirium_UI_Widget_Set_Min_Max(GUI, widget_lfo2_osc2_pitch_amount, 1,0);
			Delirium_UI_Widget_Set_Value(GUI, widget_lfo2_osc2_pitch_amount, 0);
			Delirium_UI_Widget_Set_Group_And_Member(GUI, widget_lfo2_osc2_pitch_amount, "matrix", "lfo2");
			fParameters_widget_number[dracorex_LFO2_OSC2_PITCH_AMOUNT] = widget_lfo2_osc2_pitch_amount; 
			
			
			//--------------------------------------------------------------------------------------------------------------------------------
			// FILTER ----- ------------------------------------------------------------------------------------------------------------------
			
			// FILTER ACTIVE
			int widget_filter_active = Delirium_UI_Create_Widget(GUI, deliriumUI_Switch, 0, panelX + 13.5, panelY + 1.25, 2, 2, "ACTIVE", dracorex_FILTER_ACTIVE);
			Delirium_UI_Widget_Set_Min_Max(GUI, widget_filter_active, 0,1);
			Delirium_UI_Widget_Set_Value(GUI, widget_filter_active, 0);
			Delirium_UI_Widget_Set_Group_And_Member(GUI, widget_filter_active, "matrix", "filter");
			fParameters_widget_number[dracorex_FILTER_ACTIVE] = widget_filter_active; 
			

			
			//------- FILTER CUTOFF  -----------------------------------------------------------------------
			
			int widget_filter_cutoff = Delirium_UI_Create_Widget(GUI, deliriumUI_Fader, 0, panelX + 16, panelY + 1.25, 1, 5.25, "CUTOFF", dracorex_CUTOFF);
			Delirium_UI_Widget_Set_Min_Max(GUI, widget_filter_cutoff, 1,0);
			Delirium_UI_Widget_Set_Value(GUI, widget_filter_cutoff, 0);
			Delirium_UI_Widget_Set_Group_And_Member(GUI, widget_filter_cutoff, "matrix", "filter");
			fParameters_widget_number[dracorex_CUTOFF] = widget_filter_cutoff; 
			
			//------- FILTER RESONANCE  -----------------------------------------------------------------------
			
			int widget_filter_resonance = Delirium_UI_Create_Widget(GUI, deliriumUI_Fader, 0, panelX + 18, panelY + 1.25, 1, 5.25, "RES", dracorex_RESONANCE);
			Delirium_UI_Widget_Set_Min_Max(GUI, widget_filter_resonance, 1,0);
			Delirium_UI_Widget_Set_Value(GUI, widget_filter_resonance, 0);
			Delirium_UI_Widget_Set_Group_And_Member(GUI, widget_filter_resonance, "matrix", "filter");
			fParameters_widget_number[dracorex_RESONANCE] = widget_filter_resonance; 
			
			//------- FILTER ADSR3 AMOUNT  -----------------------------------------------------------------------
			
			int widget_filter_adsr3_amount = Delirium_UI_Create_Widget(GUI, deliriumUI_Fader, 0, panelX + 20, panelY + 1.25, 1, 5.25, "ADSR3", dracorex_FILTER_ADSR3_AMOUNT);
			Delirium_UI_Widget_Set_Min_Max(GUI, widget_filter_adsr3_amount, 1,0);
			Delirium_UI_Widget_Set_Value(GUI, widget_filter_adsr3_amount, 0);
			Delirium_UI_Widget_Set_Group_And_Member(GUI, widget_filter_adsr3_amount, "matrix", "filter");
			fParameters_widget_number[dracorex_FILTER_ADSR3_AMOUNT] = widget_filter_adsr3_amount; 
			
			//--------------------------------------------------------------------------------------------------------------------------------
			// FX ----- ------------------------------------------------------------------------------------------------------------------
			
			// CHORUS ACTIVE
			int widget_chorus_active = Delirium_UI_Create_Widget(GUI, deliriumUI_Switch, 0, panelX + 13.5, panelY + 1.25, 2, 2, "CHORUS", dracorex_CHORUS_ACTIVE);
			Delirium_UI_Widget_Set_Min_Max(GUI, widget_chorus_active, 0,1);
			Delirium_UI_Widget_Set_Value(GUI, widget_chorus_active, 0);
			Delirium_UI_Widget_Set_Group_And_Member(GUI, widget_chorus_active, "matrix", "fx");
			fParameters_widget_number[dracorex_CHORUS_ACTIVE] = widget_chorus_active; 
			
			// GLOBAL PANEL ----------------------------------------------------------------------------------------------------------------
			
			//-------- MASTER TUNE -----------------------------------------------------------------------
			int widget_master_tuning = Delirium_UI_Create_Widget(GUI, deliriumUI_Knob, 0, panelX + 18,  panelY + 7, 3.5, 3.5, "TUNING", dracorex_MASTER_TUNING);
			Delirium_UI_Widget_Set_Min_Max(GUI, widget_master_tuning, -7,7);
			Delirium_UI_Widget_Set_Value(GUI, widget_master_tuning, 0.6);
			Delirium_UI_Widget_Set_Default_Value(GUI, widget_master_tuning, 0.6);
			Delirium_UI_Widget_Set_Group_And_Member(GUI, widget_master_tuning, "global", "");
			fParameters_widget_number[dracorex_MASTER_TUNING] = widget_master_tuning; 
			
			
			//-------- MASTER VOLUME -----------------------------------------------------------------------
			int widget_volume = Delirium_UI_Create_Widget(GUI, deliriumUI_Knob, 0, panelX + 21.5, panelY + 7, 3.5, 3.5, "VOLUME", dracorex_VOLUME);
			Delirium_UI_Widget_Set_Min_Max(GUI, widget_volume, 0,2);
			Delirium_UI_Widget_Set_Value(GUI, widget_volume, 0.6);
			Delirium_UI_Widget_Set_Default_Value(GUI, widget_volume, 0.6);
			Delirium_UI_Widget_Set_Group_And_Member(GUI, widget_volume, "global", "");
			fParameters_widget_number[dracorex_VOLUME] = widget_volume; 
			
	

			// ADSR3 - FILTER ENVELOPE --------------------------------------------------------------------------------------------------------

			int widget_filter_env = Delirium_UI_Create_Widget(GUI, deliriumUI_ADSR, 0,panelX + 13.5,  panelY + 11, 5, 3,"ADSR3",dracorex_ADSR3_ATTACK); 
			Delirium_UI_Widget_Set_Group_And_Member(GUI, widget_filter_env, "global", "");	
			fParameters_widget_number[dracorex_ADSR3_ATTACK] = widget_filter_env; 
			fParameters_widget_number[dracorex_ADSR3_ATTACK+1] = widget_filter_env; 
			fParameters_widget_number[dracorex_ADSR3_ATTACK+2] = widget_filter_env; 
			fParameters_widget_number[dracorex_ADSR3_ATTACK+3] = widget_filter_env; 

			//-------- ADSR4 ------------------------------------------------------------------------------------------------

			int widget_adsr4 = Delirium_UI_Create_Widget(GUI, deliriumUI_ADSR, 0,panelX + 19.5,  panelY + 11, 5, 3,"ADSR4",dracorex_ADSR4_ATTACK); 
			Delirium_UI_Widget_Set_Group_And_Member(GUI, widget_adsr4, "global", "");	
			fParameters_widget_number[dracorex_ADSR4_ATTACK] = widget_adsr4; 
			fParameters_widget_number[dracorex_ADSR4_ATTACK+1] = widget_adsr4; 
			fParameters_widget_number[dracorex_ADSR4_ATTACK+2] = widget_adsr4; 
			fParameters_widget_number[dracorex_ADSR4_ATTACK+3] = widget_adsr4; 
			

			

			// loadSymbols();
			// searchPresets();
					
					
			//-----------------------------

			widget_categories_list = 65536;
			widget_presets_list = 65536;
			load_wavetables();
			GUI->draw_flag = true;					
			GUI->drag = 0;
			GUI->current_widget = -1;	
			
			Delirium_UI_Group_Set_Active_Widgets(GUI);
		}

		//------------------------------------------------------------------------------------------------------

		~dracorexUI()
		{
		
			int widget_number = -1;
		
			for (int w=0; w<GUI->Widgets.size(); w++)
			{
				if (GUI->Widgets[w]->type == deliriumUI_Selector)
					widget_number = w;
			}
			
			if (widget_number > -1)
			{
				for (int w=0; w<GUI->Widgets[widget_number]->wavetables.size(); w++)
				{
					free( GUI->Widgets[widget_number]->wavetables[w].buffer );
					cout << "Unloading: " << GUI->Widgets[widget_number]->wavetables[w].name << endl;
				}
			}
				
			Delirium_UI_Cleanup(GUI);
	
		}
		

		//------------------------------------------------------------------------------------------------------

		vector<string> split (const string &s, char delim) {
			    vector<string> result;
			    stringstream ss (s);
			    string item;
	
			    while (getline (ss, item, delim)) {
			        result.push_back (item);
				    }

		    return result;
		}
		
		//------------------------------------------------------------------------------------------------------
		
		void load_wavetables()
		{
			string user_path = getenv("USER");
			stringstream wave_path;
			stringstream wave_file;
			struct dirent *d;
			struct stat st;
			
			DIR *dr;
			wave_path.str("");
			wave_path << "/home/" << user_path << "/.lv2/thunderox_dracorex.lv2/waves";
			
			dr = opendir(wave_path.str().c_str());
			
			 
			if (dr != NULL)
			{
				for( d=readdir(dr); d!=NULL; d=readdir(dr)) // List all files here
				{
					wave_file.str("");
					wave_file << wave_path.str() << "/" << d->d_name;

					FILE* fp = fopen (wave_file.str().c_str(),"r");
					fseek(fp, 0, SEEK_END); // We can use rewind(fp); also
					
					if (ftell(fp) == 35360)
					{					
						long length = (ftell(fp) - 80 )/ 4;
						cout << "Loading Waveform: " << wave_file.str() << " - " << ftell(fp) << endl;
						wavetable new_waveform;
						new_waveform.name = d->d_name;
						new_waveform.buffer = (float *)malloc(length*sizeof(float)) ;
						
						fseek(fp, 80, SEEK_SET);
						fread(new_waveform.buffer ,1, length*sizeof(float), fp);
						new_waveform.length = length;
						
						for (int w=0; w<GUI->Widgets.size(); w++)
						{
							if (GUI->Widgets[w]->type == deliriumUI_Selector)
							{
								GUI->Widgets[w]->wavetables.push_back(new_waveform);
							}
						}
					}
					fclose(fp);
					
					for (int w=0; w<GUI->Widgets.size(); w++)
					{
						if (GUI->Widgets[w]->type == deliriumUI_Selector)
						{
							GUI->Widgets[w]->max = GUI->Widgets[w]->wavetables.size()-1;
							GUI->Widgets[w]->increment = 1;
							sort(GUI->Widgets[w]->wavetables.begin(),GUI->Widgets[w]				
								->wavetables.end(),alphasort_wavetables() );	
						}
					}
				}
				
				
			}
			
			
			
		}
		
		//------------------------------------------------------------------------------------------------------
		void loadSymbols()
		{
			string lv2_path = getenv("LV2_PATH");
			string line;
			struct dirent *d;
			struct stat st;
			DIR *dr;
			stringstream dracorex_ttl_file_name;
			number_of_symbols = 0;
					
			for (int x=0; x<200; x++) symbols.push_back("");
			
			vector<string> v = split (lv2_path, ':');
			
			for (int z=0; z<v.size(); z++)
			{
				dracorex_ttl_file_name.str("");
				dracorex_ttl_file_name << v[z] << "/thunderox_dracorex.lv2/thunderox_dracorex_dsp.ttl";


				ifstream dracorex_ttl_file(dracorex_ttl_file_name.str().c_str(), ios::in);
				
				while (getline(dracorex_ttl_file,line))  
				{
				
					stringstream symbol_name;
					symbol_name.str("");
					int symbol_index;
					bool symbol_found = false;
						
					int search_pos = line.rfind("lv2:symbol");
					if (search_pos > 0)
					{
						symbol_found = true;
						int add_this_char = 0;
						
						for (int char_number = 0; char_number < line.length(); char_number++)
						{
							if (line[char_number] == 34) add_this_char = 1 - add_this_char;
							else if (add_this_char) symbol_name << line[char_number];
						}
						
						

					}					
					
					search_pos = line.rfind("lv2:index");
					
					if (search_pos > 0)
					{
						istringstream (line.substr(search_pos+10)) >> symbol_index;
					}
					
					if (symbol_found)
					{
						symbols[symbol_index] = symbol_name.str();
						if (symbol_index > number_of_symbols) number_of_symbols = symbol_index;
					}	
				}
				
				dracorex_ttl_file.close();
			}
		}


		
		//------------------------------------------------------------------------------------------------------
		
		void searchPresets()
		{
			string lv2_path = getenv("LV2_PATH");
			stringstream ss;
			struct dirent *d, *pr_d;
			struct stat st;
			DIR *dr, *prDr;
			stringstream file_name_and_path;
			stringstream path_name;
			
			/*
		
			ifstream categories_file("/usr/lib/lv2/thunderox_dracorex.lv2/dracorex_presets.lv2/categories.txt");
			string line;
		
			int category_number = 0;
			

			while (getline(categories_file,line))
			{
				category new_category;
				new_category.number = category_number++;
				new_category.name = line;
				categories.push_back(new_category);
				Delirium_UI_Widget_List_Add_Item(GUI, widget_categories_list, line);
			}  
			categories_file.close();	

			*/

			// preset_category_file.open("test.txt");
			vector<string> v = split (lv2_path, ':');
			    
			for (int z=0; z<v.size(); z++)
			{
				dr = opendir(v[z].c_str()); // search through LV2 folders in LV2_PATH 
				
				if (dr!=NULL)
				{
					for( d=readdir(dr); d!=NULL; d=readdir(dr)) // List all files here
					{
						string file_name = d->d_name;
							if (file_name == "thunderox_dracorex.lv2")
							{
								file_name = "thunderox_dracorex.lv2/dracorex_presets.lv2";
							}
							
							if (stat(file_name.c_str(),&st)) // Look in each folder
							{
								path_name.str("");
								path_name << v[z] << "/" << file_name;
								prDr = opendir(path_name.str().c_str()); // Read file see if it applies to our plugin

								if (prDr!=NULL)
								{
									for( pr_d=readdir(prDr); pr_d!=NULL; pr_d=readdir(prDr))
									{
										file_name_and_path.str("");
										file_name_and_path <<  v[z] << "/" << file_name << "/" << pr_d->d_name;
										
										int file_is_ttl = file_name_and_path.str().rfind(".ttl");
										int file_is_manifest = file_name_and_path.str().rfind("manifest.ttl");
										int file_is_dracorex = file_name_and_path.str().rfind("dracorex.ttl");
										
										if (file_is_ttl > 0 && file_is_manifest < 0 && file_is_dracorex < 0)
										{
																
											bool is_dracorex_preset = false;
											ifstream preset_file(file_name_and_path.str().c_str(), ios::in);
											string line;
									
											while (getline(preset_file,line))  
											{
												int search_pos = line.rfind("dracorex");
												if (search_pos > 0)
												{
													is_dracorex_preset = true;

												}
												
												/*
												
												search_pos = line.rfind("preset_category");
												if (search_pos > 0)
												{
													getline(preset_file,line); 
													search_pos = line.rfind("pset:value");
													istringstream value_str;
													istringstream ( line.substr(search_pos + 11, line.length()+1)
														) >> category_number;
												}
												
												*/
										
											}
																			
											preset_file.close();
											if (is_dracorex_preset)
											{
												preset new_preset;
												new_preset.file = file_name_and_path.str().c_str();
												string preset_name = pr_d->d_name;
												new_preset.name = preset_name.substr(0,preset_name.length()-4);
												
												string category_name = Find_Preset_Category(new_preset.file);
												bool category_found = false;
												
												for (int x=0; x<categories.size(); x++)
												{
													if (categories[x].name == category_name)
													{
														categories[x].presets.push_back(new_preset);
														category_found = true;
													}
												}
												
												if (!category_found)
												{
													category new_category;
													new_category.name = category_name;
													new_category.presets.push_back(new_preset);			
													categories.push_back(new_category);																																					
												}
												
												
												
												
												// Find_Preset_Category(new_preset.file);
											}	
										}			
								}
								closedir(prDr);
							}
						}
					}
					closedir(dr);
				}				
			}		

			/*

			for (int pr=0; pr<categories[0].presets.size(); pr++)
			{			
				Delirium_UI_Widget_List_Add_Item(GUI, widget_presets_list, categories[0].presets[pr].name);
			}
			
			*/
			
			sort(categories.begin(),categories.end(),alphasort_category());
			
			for (int x=0; x<categories.size(); x++)
			{
				Delirium_UI_Widget_List_Add_Item(GUI, widget_categories_list, categories[x].name);
			}
			
			for (int x=0; x<categories.size(); x++)
			{
				sort(categories[x].presets.begin(),categories[x].presets.end(),alphasort_preset());	
			}
			
		}
		
		//--------------------------------------------------------------------------------------
		// GET SYMBOL INDEX NUMBER (PORT)
		
		int getSymbolIndex(string symbol_name)
		{
			int symbol_index = -1;
			
			for (int x=0; x<number_of_symbols; x++)
			{
				if (symbols[x] == symbol_name)
					symbol_index = x;
			}	
			return symbol_index;
		}
		
		//--------------------------------------------------------------------------------------
		// LOAD PRESET

		void loadPreset()
		{ 	
			int category_number = current_category;

			int preset_number = GUI->Widgets[widget_presets_list]->list_position
				+ GUI->Widgets[widget_presets_list]->list_scroll;
				
			if ( preset_number > categories[category_number].presets.size()-1
				|| category_number > categories.size()-1
				|| categories[category_number].presets.size() == 0) return;
				
			string preset_path_and_file_name = categories[category_number].presets[preset_number].file;			

			ifstream preset_file;
	
			cout << preset_path_and_file_name << endl;
			preset_file.open(preset_path_and_file_name );

			string preset_symbol;

			string temp_str;
			string line;
			double preset_value;
			
			while (getline(preset_file,line))  
			{
				int preset_index = line.rfind("lv2:symbol ");
				if (preset_index > 0) preset_symbol = line.substr(preset_index + 12, line.length() - preset_index - 15);

				preset_index = line.rfind("pset:value");
				if (preset_index > 0) istringstream ( line.substr(preset_index + 11, line.length()+1 ) ) >> preset_value;

				if (preset_index > 0)
				{
				
					int symbol_index = getSymbolIndex(preset_symbol) - 3;
					
					if (symbol_index > -1) 
					{
						// /*
					
						if (symbol_index >= 36 && symbol_index <=40 ) 
						 {
						 	preset_value = 1 - preset_value;
						 }
						 
						if (symbol_index >= 44 && symbol_index <=47 ) 
						 {
						 	preset_value = 1 - preset_value;
						 }
						 
						if (symbol_index >= 52 && symbol_index <=55 ) 
						 {
						 	preset_value = 1 - preset_value;
						 }
						 
						 // */
						 
						parameterChanged(symbol_index, preset_value);
						 
					}

				} 
			}
			
		
						
			preset_file.close();

		}

		//------------------------------------------------------------------------------------------------------
		string Find_Preset_Category(string file_name)
		{
		
			ifstream preset_file;
			preset_file.open(file_name );

			
			string category_name = "Unsorted";
			string line;
			
			while (getline(preset_file,line))  
			{

				int preset_index = line.rfind("pset:bank");

				if (preset_index > 0)
				{
				
					int start_index = line.rfind("<");
					int end_index = line.rfind(">") - start_index - 1;
					category_name =  line.substr(start_index+1, end_index);
					// cout << category_name << endl;
					

				} 
			}
			
		
						
			preset_file.close();
		
			return category_name;
		}

		//------------------------------------------------------------------------------------------------------

		 void onCairoDisplay(const CairoGraphicsContext& context)
		{
			cr = context.handle;
			
			Delirium_UI_Display_All(GUI, cr);
			
			
		}
		
		//------------------------------------------------------------------------------------------------------

		bool onMouse(const MouseEvent& ev) override
		{
			

			if (ev.button == 1)
			{
				Delirium_UI_MouseOver(GUI, cr, ev.pos.getX(), ev.pos.getY());
				// if (Delirium_UI_Widget_Get_Type(GUI) == deliriumUI_Panel) return true;
				Delirium_UI_Left_Button_Press(GUI, cr, ev.pos.getX(), ev.pos.getY());
				repaint();

				
				int parameter_number = Delirium_UI_Widget_Get_Parameter_Number(GUI);
				
				if ( GUI->current_widget == widget_categories_list)
				{
					int category_number = GUI->Widgets[widget_categories_list]->list_position
						+ GUI->Widgets[widget_categories_list]->list_scroll;
				
					GUI->Widgets[widget_presets_list]->list_items.clear();
					GUI->Widgets[widget_presets_list]->list_scroll = 0;
					current_category = category_number;

					for (int pr=0; pr<categories[category_number].presets.size(); pr++)
					{			
						Delirium_UI_Widget_List_Add_Item(GUI, widget_presets_list, categories[category_number].presets[pr].name);
					}
					
				}

				
				
				if ( GUI->current_widget == widget_presets_list )
				{
					loadPreset();
				}
				
				if (parameter_number > -1)
				{	
				
					float value = Delirium_UI_Widget_Get_Value(GUI);
					setParameterValue(parameter_number, value);
			
					
					if (Delirium_UI_Widget_Get_Type(GUI) == deliriumUI_Fader_Route)
					{
						setParameterValue(parameter_number+1, Delirium_UI_Widget_Get_Route_Number(GUI));
					}
					 
				}

			}


			if (ev.button == 2)
			{

				int parameter_number = Delirium_UI_Widget_Get_Parameter_Number(GUI);
				if (parameter_number > -1)
				{
	
					setParameterValue(parameter_number, 0);
					Delirium_UI_Middle_Button_Press(GUI);	
					repaint();	
					
				
				}
			}


			return true;
		}

		//------------------------------------------------------------------------------------------------------

		bool onMotion(const MotionEvent& ev)
		{

			Delirium_UI_MouseOver(GUI, cr, ev.pos.getX(), ev.pos.getY());
			


			repaint();	

			int parameter_number = Delirium_UI_Widget_Get_Parameter_Number(GUI);
	

			if (parameter_number > -1)
			{	
				float value = Delirium_UI_Widget_Get_Value(GUI);
				setParameterValue(parameter_number, value);
				
				if (Delirium_UI_Widget_Get_Type(GUI) == deliriumUI_Fader_Route)
				{
					setParameterValue(parameter_number+1, Delirium_UI_Widget_Get_Route_Number(GUI));
				}
				

					 
			}
			return true;

		}

		//------------------------------------------------------------------------------------------------------

		bool onScroll(const ScrollEvent& ev)
		{
			float delta = ev.delta.getY();

			int current_widget = GUI->current_widget;

			if (current_widget > 0)
			{	
				if (GUI->Widgets[current_widget]->type == deliriumUI_Panel) return true;
			
				GUI->Widgets[current_widget]->Mouse_Scroll(ev.pos.getX(), ev.pos.getY(), delta);
				Delirium_UI_Convert_Value_To_Range(GUI, current_widget);
				GUI->Widgets[current_widget]->Draw(cr);
				repaint();
			}

			int parameter_number = Delirium_UI_Widget_Get_Parameter_Number(GUI);

			if (parameter_number > -1)
			{	
				float value = Delirium_UI_Widget_Get_Value(GUI);
				setParameterValue(parameter_number, value);

				if (Delirium_UI_Widget_Get_Type(GUI) == deliriumUI_Fader_Route)
				{
					setParameterValue(parameter_number+1, Delirium_UI_Widget_Get_Route_Number(GUI));
				}
				

			}

			return true;
		}
		



	protected:
	
		//--------------------------------------------------------------------------------------------------------
		void programLoaded(uint32_t index) override
		{
			GUI->draw_flag = true;
			Delirium_UI_Display_All(GUI, cr);
		}
		
		
		/* --------------------------------------------------------------------------------------------------------
		* DSP/Plugin Callbacks 
		A parameter has changed on the plugin side.
		This is called by the host to inform the UI about parameter changes.
		*/

		void parameterChanged(uint32_t index, float value) override
		{					



			// get widget number connected to this parameter
			int widget_number = fParameters_widget_number[index];

			if (widget_number > -1)
			{
				Delirium_UI_Widget_Base* wdg = (Delirium_UI_Widget_Base*)GUI->Widgets[widget_number];
				
				if (index == wdg->parameter_number && wdg->type == deliriumUI_ADSR) wdg->current_value = 0;
				
				if (index > wdg->parameter_number && wdg->type == deliriumUI_ADSR)
				{
					wdg->current_value = ( index - wdg->parameter_number);
				}
				
				if (wdg->type == deliriumUI_ADSR)
				cout << widget_number << " - " <<  wdg->current_value << endl;
				
				if ((int)index > wdg->parameter_number && wdg->type == deliriumUI_Fader_Route)
					wdg->route_number = value;

				if ((int)index == wdg->parameter_number && wdg->type == deliriumUI_Fader_Route)
					Delirium_UI_Widget_Set_Value(GUI, widget_number, value);
		
				
				if (wdg->type != deliriumUI_Fader_Route) Delirium_UI_Widget_Set_Value(GUI, widget_number, value);
				
			
			
			}
			// trigger repaint

			fParameters[index] = value;
			setParameterValue(index, value);
			
			GUI->draw_flag = true;		
			repaint();
			
			
		}

	private:

		Delirium_UI_Surface* GUI;
		cairo_t* cr;
		float fParameters[kParameterCount+3];
		int fParameters_widget_number[kParameterCount+3];
		
		vector <string> symbols;
		int number_of_symbols;
		
		struct preset
		{
			string name;
			string file;
		};
		
		struct category
		{
			string name;
			vector <preset> presets;
		};
		
		struct alphasort_preset
		{
			inline bool operator() (const preset& struct1, const preset& struct2)
			{
				return (struct1.name < struct2.name);
			}
		};

		vector <category> categories;
		
		int widget_categories_list;
		int widget_presets_list;
		int current_category;


		struct alphasort_category
		{
			inline bool operator() (const category& struct1, const category& struct2)
			{
				return (struct1.name < struct2.name);
			}
		};
		
		struct alphasort_wavetables
		{
			inline bool operator() (const wavetable& struct1, const wavetable& struct2)
			{
				return (struct1.name < struct2.name);
			}
		};
		
		/*  Set our UI class as non-copyable and add a leak detector just in case. */
    DISTRHO_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(dracorexUI)

};

UI* createUI()
{
    return new dracorexUI;
}

END_NAMESPACE_DISTRHO


