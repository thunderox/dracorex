

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

		dracorexUI() : UI(640, 480)
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
			
			
			int osc_panel = Delirium_UI_Create_Widget(GUI, deliriumUI_Panel, 0, panelX+0.5, panelY, 12,14, "OSCILLATORS", -1);
			Delirium_UI_Widget_Set_Group_And_Member(GUI, osc_panel, "global", "");

						
			// OSCILATOR NAVIGATION ONE / TWO -----------------------------------------------------------------------
			
			int oscillators_group = Delirium_UI_Group_Create(GUI, "oscillators");
			Delirium_UI_Group_Add_Member(GUI, "oscillators", "osc1");
			Delirium_UI_Group_Add_Member(GUI, "oscillators", "osc2");
			
			int nav_oscillators = Delirium_UI_Create_Widget(GUI,  deliriumUI_Tabbed_Navigator,
				0, panelX + 0.5, panelY + 0.25, 12,0.6,"",-1);			
			Delirium_UI_Group_Add_Navigator_Tab(GUI, nav_oscillators, "OSC-1", "oscillators", "osc1");
			Delirium_UI_Group_Add_Navigator_Tab(GUI, nav_oscillators, "OSC-2", "oscillators", "osc2");	
			
			Delirium_UI_Group_Set_Visible_member(GUI, "oscillators", "osc1");
			
			Delirium_UI_Widget_Set_Group_And_Member(GUI, nav_oscillators, "global", "");
					

			//------- OSCILLATOR ONE - ACTIVE -----------------------------------------------------------------------
			int widget_osc1_active = Delirium_UI_Create_Widget(GUI, deliriumUI_Switch, 0, panelX + 0.5, panelY + 1.25, 1.75, 2, "OSC-1", dracorex_OSC1_ACTIVE);

			Delirium_UI_Widget_Set_Min_Max(GUI, widget_osc1_active, 0,1);
			Delirium_UI_Widget_Set_Value(GUI, widget_osc1_active, 1);
			Delirium_UI_Widget_Set_Group_And_Member(GUI, widget_osc1_active, "oscillators", "osc1");			
			fParameters_widget_number[dracorex_OSC1_ACTIVE] = widget_osc1_active; 
			
			//------- OSCILLATOR ONE - DETUNE -----------------------------------------------------------------------

			int widget_osc1_detune = Delirium_UI_Create_Widget(GUI, deliriumUI_Fader, 0, panelX + 1, panelY + 4, 1, 5.25, "DETUNE", dracorex_OSC1_TUNING);
			Delirium_UI_Widget_Set_Min_Max(GUI, widget_osc1_detune, 7,-7);
			Delirium_UI_Widget_Set_Value(GUI, widget_osc1_detune, 0);
			Delirium_UI_Widget_Set_Group_And_Member(GUI, widget_osc1_detune, "oscillators", "osc1");
			fParameters_widget_number[dracorex_OSC1_TUNING] = widget_osc1_detune;
			
			//------- Oscilator One - ADSR2 to OSC 1 Pitch  -----------------------------------------------------------------------
			
			int widget_adsr2_amount = Delirium_UI_Create_Widget(GUI, deliriumUI_Fader, 0, panelX + 3, panelY + 4, 1, 5.25, "ADSR2", dracorex_OSC1_PITCH_ADSR2);
			Delirium_UI_Widget_Set_Min_Max(GUI, widget_adsr2_amount, 1,0);
			Delirium_UI_Widget_Set_Value(GUI, widget_adsr2_amount, 0);
			Delirium_UI_Widget_Set_Group_And_Member(GUI, widget_adsr2_amount, "oscillators", "osc1");
			fParameters_widget_number[dracorex_OSC1_PITCH_ADSR2] = widget_adsr2_amount; 
			
			//------- Oscilator One - ADSR3 to OSC 1 Pitch  -----------------------------------------------------------------------
			
			int widget_adsr3_amount = Delirium_UI_Create_Widget(GUI, deliriumUI_Fader, 0, panelX + 5, panelY + 4, 1, 5.25, "ADSR3", dracorex_OSC1_PITCH_ADSR3);
			Delirium_UI_Widget_Set_Min_Max(GUI, widget_adsr3_amount, 1,0);
			Delirium_UI_Widget_Set_Value(GUI, widget_adsr3_amount, 0);
			Delirium_UI_Widget_Set_Group_And_Member(GUI, widget_adsr3_amount, "oscillators", "osc1");
			fParameters_widget_number[dracorex_OSC1_PITCH_ADSR3] = widget_adsr3_amount; 
			

			//------- Oscilator One - ADSR1 - OSC1 AMP ENVELOPE -----------------------------------------------------------------------

			int widget_amp_env = Delirium_UI_Create_Widget(GUI, deliriumUI_ADSR, 0, panelX + 1, panelY + 10, 5, 3,"ADSR1 - AMP",dracorex_AMP_ATTACK); 
			Delirium_UI_Widget_Set_Group_And_Member(GUI, widget_amp_env, "oscillators", "osc1");	
			fParameters_widget_number[dracorex_AMP_ATTACK] = widget_amp_env; 
			fParameters_widget_number[dracorex_AMP_ATTACK+1] = widget_amp_env; 
			fParameters_widget_number[dracorex_AMP_ATTACK+2] = widget_amp_env; 
			fParameters_widget_number[dracorex_AMP_ATTACK+3] = widget_amp_env; 
			
			//------- Oscilator One - ADSR2 - OSC1 WAVE ENVELOPE -----------------------------------------------------------------------
			
			int widget_wave_env = Delirium_UI_Create_Widget(GUI, deliriumUI_ADSR, 0, panelX + 7, panelY + 10, 5, 3,"ADSR2 - WAVE",dracorex_WAVE_ATTACK); 
			Delirium_UI_Widget_Set_Group_And_Member(GUI, widget_wave_env, "oscillators", "osc1");	
			fParameters_widget_number[dracorex_WAVE_ATTACK] = widget_wave_env; 
			fParameters_widget_number[dracorex_WAVE_ATTACK+1] = widget_wave_env; 
			fParameters_widget_number[dracorex_WAVE_ATTACK+2] = widget_wave_env; 
			fParameters_widget_number[dracorex_WAVE_ATTACK+3] = widget_wave_env; 
			
			//-------- MASTER VOLUME -----------------------------------------------------------------------
			int widget_volume = Delirium_UI_Create_Widget(GUI, deliriumUI_Knob, 0, panelX + 18, panelY +9, 4, 4, "VOLUME", dracorex_VOLUME);
			Delirium_UI_Widget_Set_Min_Max(GUI, widget_volume, 0,2);
			Delirium_UI_Widget_Set_Value(GUI, widget_volume, 0.6);
			Delirium_UI_Widget_Set_Default_Value(GUI, widget_volume, 0.6);
			Delirium_UI_Widget_Set_Group_And_Member(GUI, widget_volume, "global", "");
			fParameters_widget_number[dracorex_VOLUME] = widget_volume; 
			
			//-------- MASTER TUNE -----------------------------------------------------------------------
			int widget_master_tuning = Delirium_UI_Create_Widget(GUI, deliriumUI_Knob, 0, panelX + 14, panelY +9, 4, 4, "TUNING", dracorex_MASTER_TUNING);
			Delirium_UI_Widget_Set_Min_Max(GUI, widget_master_tuning, -7,7);
			Delirium_UI_Widget_Set_Value(GUI, widget_master_tuning, 0.6);
			Delirium_UI_Widget_Set_Default_Value(GUI, widget_master_tuning, 0.6);
			Delirium_UI_Widget_Set_Group_And_Member(GUI, widget_master_tuning, "global", "");
			fParameters_widget_number[dracorex_MASTER_TUNING] = widget_master_tuning; 

			// loadSymbols();
			// searchPresets();
					
					
			//-----------------------------

			GUI->draw_flag = true;					
			GUI->drag = 0;
			GUI->current_widget = -1;	
			
			Delirium_UI_Group_Set_Active_Widgets(GUI);
		}

		//------------------------------------------------------------------------------------------------------

		~dracorexUI()
		{
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
				return true;

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
		
		/*  Set our UI class as non-copyable and add a leak detector just in case. */
    DISTRHO_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(dracorexUI)

};

UI* createUI()
{
    return new dracorexUI;
}

END_NAMESPACE_DISTRHO


