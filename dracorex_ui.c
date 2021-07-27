/*
  Copyright 2015 ~ 2019 Nick Bailey
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "dracorex.h"

#include <cairo/cairo.h>

#include "lv2/lv2plug.in/ns/extensions/ui/ui.h"
#include "deliriumUI/deliriumUI.h"

float* wavetable_buffer;
int waveform_length;
char* wave_names[32];
int number_of_waves;

int button_oscillators = -1;
int button_lfo = -1;
int button_filter = -1;
int button_fx = -1;


#define dracorex_UI_URI "http://nickbailey.co.nr/plugins/dracorex#ui"

void upper_string(char s[]) {
   int c = 0;
   
   while (s[c] != '\0') {
      if (s[c] >= 'a' && s[c] <= 'z') {
         s[c] = s[c] - 32;
      }
      c++;
   }
}

typedef struct {
	PuglView*            view;
	LV2UI_Write_Function write;
	LV2UI_Controller     controller;
	int                  width;
	int                  height;
	deliriumUI	     deliriumUI_window;
	float                brightness;
	bool                 exit;
} dracorexUI;

static void
onReshape(PuglView* view, int width, int height)
{
	dracorexUI* self = (dracorexUI*)puglGetHandle(view);

	setDeliriumUICurrentWindowSize(&self->deliriumUI_window, width, height);
}


static void
onDisplay(PuglView* view)
{
	dracorexUI* self = (dracorexUI*)puglGetHandle(view);
	cairo_t* cr = puglGetContext(view);

	displayAllDeliriumUIWidgets(&self->deliriumUI_window,cr);
}

static void
onKeyboard(PuglView* view, bool press, uint32_t key)
{
	dracorexUI* self = (dracorexUI*)puglGetHandle(view);
	cairo_t* cr = puglGetContext(view);

	if (press) {

		if (key==27)
		{
			self->exit = true;
		}


		if (key=='r')
		{
			displayAllDeliriumUIWidgets(&self->deliriumUI_window,cr);
		}

		fprintf(stderr, "Keyboard press %c\n", key);
	} else {
		fprintf(stderr, "Keyboard release %c\n", key);
	}
}

static void
onMotion(PuglView* view, int x, int y)
{
	dracorexUI* self = (dracorexUI*)puglGetHandle(view);
	cairo_t* cr = puglGetContext(view);

	// fprintf(stderr, "Motion: %d,%d\n", x, y);
	isMouseOverDeliriumUIWidget(&self->deliriumUI_window, cr, x, y);
}

static void
onMouse(PuglView* view, int button, bool press, int x, int y)
{
	if (press == true && button == 0 || button > 15) return;

	dracorexUI* self = (dracorexUI*)puglGetHandle(view);
	cairo_t* cr = puglGetContext(view);
	int widgetNumber = self->deliriumUI_window.currentWidgetNumber;
	int widgetType = self->deliriumUI_window.deliriumUIWidgets[widgetNumber].type;
	int group = self->deliriumUI_window.deliriumUIWidgets[widgetNumber].group;

	self->deliriumUI_window.mouseButton[button] = press;

	if (widgetType == deliriumUI_Button && press == true && button == 1)
	{

		if (widgetNumber+1 == button_oscillators)
		{
			self->deliriumUI_window.group_visible[1] = true;
			self->deliriumUI_window.group_visible[2] = false;
			self->deliriumUI_window.group_visible[3] = false;
			self->deliriumUI_window.group_visible[4] = false;
			return;
		}

		if (widgetNumber+1 == button_lfo)
		{
			self->deliriumUI_window.group_visible[1] = false;
			self->deliriumUI_window.group_visible[2] = true;
			self->deliriumUI_window.group_visible[3] = false;
			self->deliriumUI_window.group_visible[4] = false;
			return;
		}

		if (widgetNumber+1 == button_filter)
		{
			self->deliriumUI_window.group_visible[1] = false;
			self->deliriumUI_window.group_visible[2] = false;
			self->deliriumUI_window.group_visible[3] = true;
			self->deliriumUI_window.group_visible[4] = false;
			return;
		}
		
		if (widgetNumber+1 == button_fx)
		{
			self->deliriumUI_window.group_visible[1] = false;
			self->deliriumUI_window.group_visible[2] = false;
			self->deliriumUI_window.group_visible[3] = false;
			self->deliriumUI_window.group_visible[4] = true;
			return;
		}
		
	}


	if (widgetType == deliriumUI_Wave_Selector && self->deliriumUI_window.group_visible[group])
	{
		if (press == true && button == 3) 	
		{
			incValue(&self->deliriumUI_window, cr);
			return;
		}

		if (press == true && button == 1) 	
		{
			decValue(&self->deliriumUI_window, cr);
			return;
		}
	}

	if (press == true && button == 1 && widgetType == deliriumUI_LFO)
	{
		incValue(&self->deliriumUI_window, cr);
		return;
	}

	if (press == true && button == 3 && widgetType == deliriumUI_LFO)
	{
		setValueFromMousePosition(&self->deliriumUI_window, widgetNumber, x, y);
		self->deliriumUI_window.deliriumUIWidgets[widgetNumber].pressed = true;
		return;
	}

	if (press == true && button == 3 && widgetType != deliriumUI_LFO)
	{
		zeroValue(&self->deliriumUI_window, cr);
		return;
	}

	if (press == true && button == 1 && widgetType != deliriumUI_LFO)
	{
		hasMouseClickedDeliriumUIWidget(&self->deliriumUI_window, x, y);
		return;
	}
	else
	{
		hasMouseClickedDeliriumUIWidget(&self->deliriumUI_window, -1,-1);
		return;
	}
}

static void
onScroll(PuglView* view, int x, int y, float dx, float dy)
{
	dracorexUI* self = (dracorexUI*)puglGetHandle(view);
	cairo_t* cr = puglGetContext(view);

	if (dy!=0) 
	{
		if (dy == -1)
		{
			decValue(&self->deliriumUI_window, cr);
		}

		if (dy == 1)
		{
			incValue(&self->deliriumUI_window, cr);
		}
	}
}


static LV2UI_Handle
instantiate(const LV2UI_Descriptor*   descriptor,
            const char*               plugin_uri,
            const char*               bundle_path,
            LV2UI_Write_Function      write_function,
            LV2UI_Controller          controller,
            LV2UI_Widget*             widget,
            const LV2_Feature* const* features)
{
	dracorexUI* self = (dracorexUI*)malloc(sizeof(dracorexUI));
	if (!self) {
		return NULL;
	}

	self->write      = write_function;
	self->controller = controller;
	self->width      = 640;
	self->height     = 480;
	self->exit       = false;	

	self->deliriumUI_window.deliriumUIWidgets = NULL;
	self->deliriumUI_window.numberOfUIWidgets = 1;
	self->deliriumUI_window.currentWidgetNumber = -1;

	for (int b=0; b<16; ++b)
	{
		self->deliriumUI_window.mouseButton[b] = 0;
		self->deliriumUI_window.group_visible[b] = false; 
		self->deliriumUI_window.group_visible[b+16] = false; 
	}

	setDeliriumUIGridSize(&self->deliriumUI_window, self->width, self->height, 12, 14);


	// Get parent window and resize API from features
	PuglNativeWindow parent = 0;
	LV2UI_Resize*    resize = NULL;
	for (int i = 0; features && features[i]; ++i) {
		printf("Feature %s\n", features[i]->URI );
		if (!strcmp(features[i]->URI, LV2_UI__parent)) {
			parent = (PuglNativeWindow)features[i]->data;
		} else if (!strcmp(features[i]->URI, LV2_UI__resize)) {
			resize = (LV2UI_Resize*)features[i]->data;
		}
	}

	if (!parent) {
		fprintf(stderr, "error: dracorex_ui: No parent window provided.\n");
		free(self);
		return NULL;
	}

	setControllerdeliriumUI(controller, write_function);

	// GUI - Tab Selection Buttons and persistent controls

	button_oscillators = addDeliriumUIWidget(&self->deliriumUI_window, deliriumUI_Button, 6,0.25,0.25,2,0.75,"Oscillators",-2);
	button_lfo = addDeliriumUIWidget(&self->deliriumUI_window, deliriumUI_Button, 6,2.5,0.25,2,0.75,"LFO",-2);
	button_filter = addDeliriumUIWidget(&self->deliriumUI_window, deliriumUI_Button, 6,4.75,0.25,2,0.75,"Filter",-2);
	button_fx = addDeliriumUIWidget(&self->deliriumUI_window, deliriumUI_Button, 6,7,0.25,2,0.75,"FX",-2);

	addDeliriumUIWidget(&self->deliriumUI_window, deliriumUI_Knob, 6, 5,1,2,4,"VOLUME",dracorex_VOLUME);

	// GUI - Oscillators

	addDeliriumUIWidget(&self->deliriumUI_window, deliriumUI_ADSR, 1,0.25,1.5,4,3,"ADSR 1 - AMP",dracorex_AMP_ATTACK);

	addDeliriumUIWidget(&self->deliriumUI_window, deliriumUI_Switch, 1, 0,6,1,2,"OSC 1",dracorex_OSC1_ACTIVE);
	addDeliriumUIWidget(&self->deliriumUI_window, deliriumUI_Wave_Selector, 1, 2.25,4.75,2,2,"WAVE A",dracorex_OSC1_WAVE_A);
	setMinMax(&self->deliriumUI_window, dracorex_OSC1_WAVE_A,0,7);
	addDeliriumUIWidget(&self->deliriumUI_window, deliriumUI_Wave_Selector,  1, 2.25,7.25,2,2,"WAVE B",dracorex_OSC1_WAVE_B);
	setMinMax(&self->deliriumUI_window, dracorex_OSC1_WAVE_B,0,7);
	addDeliriumUIWidget(&self->deliriumUI_window, deliriumUI_Fader, 1, 1,5,1,4,"TUNING",dracorex_OSC1_TUNING);
	setMinMax(&self->deliriumUI_window, dracorex_OSC1_TUNING, -12,12);
	addDeliriumUIWidget(&self->deliriumUI_window, deliriumUI_Fader, 1,0.25,9.75,1,4,"ADSR 2",dracorex_OSC1_PITCH_ADSR2);
	addDeliriumUIWidget(&self->deliriumUI_window, deliriumUI_Fader, 1,1.25,9.75,1,4,"ADSR 3",dracorex_OSC1_PITCH_ADSR3);

	addDeliriumUIWidget(&self->deliriumUI_window, deliriumUI_ADSR, 1, 7.75,1.5,4,3,"ADSR 2 - WAVE",dracorex_WAVE_ATTACK);

	addDeliriumUIWidget(&self->deliriumUI_window, deliriumUI_Switch, 1,7.5,6,1,2,"OSC 2",dracorex_OSC2_ACTIVE);
	addDeliriumUIWidget(&self->deliriumUI_window, deliriumUI_Wave_Selector, 1, 9.75,4.75,2,2,"WAVE A",dracorex_OSC2_WAVE_A);
	setMinMax(&self->deliriumUI_window, dracorex_OSC2_WAVE_A,0,15);
	addDeliriumUIWidget(&self->deliriumUI_window, deliriumUI_Wave_Selector,  1, 9.75,7.25,2,2,"WAVE B",dracorex_OSC2_WAVE_B);
	setMinMax(&self->deliriumUI_window, dracorex_OSC2_WAVE_B,0,15);
	addDeliriumUIWidget(&self->deliriumUI_window, deliriumUI_Fader, 1, 8.5,5,1,4,"TUNING",dracorex_OSC2_TUNING);
	setMinMax(&self->deliriumUI_window, dracorex_OSC2_TUNING, -12,12);
	addDeliriumUIWidget(&self->deliriumUI_window, deliriumUI_Fader, 1,7.25,9.75,1,4,"ADSR 2",dracorex_OSC2_PITCH_ADSR2);
	addDeliriumUIWidget(&self->deliriumUI_window, deliriumUI_Fader, 1,8.25,9.75,1,4,"ADSR 3",dracorex_OSC2_PITCH_ADSR3);

	// GUI - LFOs

	addDeliriumUIWidget(&self->deliriumUI_window, deliriumUI_Wave_Selector, 2, 0.25,5.75,2,2,"WAVE A",dracorex_LFO1_WAVE);
	addDeliriumUIWidget(&self->deliriumUI_window, deliriumUI_LFO,2,2.5,5.75,1,2,"BPM",dracorex_LFO1_SPEED);
	addDeliriumUIWidget(&self->deliriumUI_window, deliriumUI_Switch, 2,4.2,5.75,1,2,"RETRIG",dracorex_LFO1_RETRIG);
	addDeliriumUIWidget(&self->deliriumUI_window, deliriumUI_Fader, 2,0.25,8.5,1,4,"OSC1",dracorex_LFO1_OSC1_AMOUNT);
	addDeliriumUIWidget(&self->deliriumUI_window, deliriumUI_Fader, 2,1.25,8.5,1,4,"OSC2",dracorex_LFO1_OSC2_AMOUNT);
	addDeliriumUIWidget(&self->deliriumUI_window, deliriumUI_Fader, 2,2.25,8.5,1,4,"WAVE1",dracorex_LFO1_OSC1_WAVE_AMOUNT);
	addDeliriumUIWidget(&self->deliriumUI_window, deliriumUI_Fader, 2,3.25,8.5,1,4,"WAVE2",dracorex_LFO1_OSC2_WAVE_AMOUNT);
	addDeliriumUIWidget(&self->deliriumUI_window, deliriumUI_Fader, 2,4.25,8.5,1,4,"FILTER",dracorex_LFO1_FILTER_AMOUNT);
	setMinMax(&self->deliriumUI_window, dracorex_LFO1_SPEED,1,999);
	setIncrement(&self->deliriumUI_window, dracorex_LFO1_SPEED,1);

	addDeliriumUIWidget(&self->deliriumUI_window, deliriumUI_Wave_Selector, 2, 6.25,5.75,2,2,"WAVE A",dracorex_LFO2_WAVE);
	addDeliriumUIWidget(&self->deliriumUI_window, deliriumUI_LFO,2,8.5,5.75,1,2,"BPM",dracorex_LFO2_SPEED);
	addDeliriumUIWidget(&self->deliriumUI_window, deliriumUI_Switch, 2,10.2,5.75,1,2,"RETRIG",dracorex_LFO1_RETRIG);
	addDeliriumUIWidget(&self->deliriumUI_window, deliriumUI_Fader, 2,6.25,8.5,1,4,"OSC1",dracorex_LFO2_OSC1_AMOUNT);
	addDeliriumUIWidget(&self->deliriumUI_window, deliriumUI_Fader, 2,7.25,8.5,1,4,"OSC2",dracorex_LFO2_OSC2_AMOUNT);
	addDeliriumUIWidget(&self->deliriumUI_window, deliriumUI_Fader, 2,8.25,8.5,1,4,"WAVE1",dracorex_LFO2_OSC1_WAVE_AMOUNT);
	addDeliriumUIWidget(&self->deliriumUI_window, deliriumUI_Fader, 2,9.25,8.5,1,4,"WAVE2",dracorex_LFO2_OSC2_WAVE_AMOUNT);
	addDeliriumUIWidget(&self->deliriumUI_window, deliriumUI_Fader, 2,10.25,8.5,1,4,"FILTER",dracorex_LFO2_FILTER_AMOUNT);
	setMinMax(&self->deliriumUI_window, dracorex_LFO2_SPEED,1,999);
	setIncrement(&self->deliriumUI_window, dracorex_LFO2_SPEED,1);

	// GUI - Filters

	addDeliriumUIWidget(&self->deliriumUI_window, deliriumUI_Switch, 3,0.25,2,1,2,"FILT",dracorex_FILTER_ACTIVE);
	addDeliriumUIWidget(&self->deliriumUI_window, deliriumUI_Fader, 3, 1.25,1.5,1,4,"CUTOFF",dracorex_CUTOFF);
	addDeliriumUIWidget(&self->deliriumUI_window, deliriumUI_Fader, 3, 2.25,1.5,1,4,"RES",dracorex_RESONANCE);

	addDeliriumUIWidget(&self->deliriumUI_window, deliriumUI_ADSR, 3,7.75,1.5,4,3,"ADSR 3 - FILTER",dracorex_ADSR3_ATTACK);	

	// GUI - FX

	addDeliriumUIWidget(&self->deliriumUI_window, deliriumUI_Switch, 4,6,11,1,2,"CHORUS",dracorex_CHORUS_ACTIVE);

	self->deliriumUI_window.group_visible[1] = true;
	self->deliriumUI_window.group_visible[6] = true;
	set_bundle_path((char*)bundle_path);

	// Set up pugl window

	self->view = puglInit(NULL, NULL);
	puglInitWindowParent(self->view, parent);
	puglInitWindowSize(self->view, 1280,800);
	puglInitResizable(self->view, true);
	puglInitContextType(self->view, PUGL_CAIRO);
	puglIgnoreKeyRepeat(self->view, false);

	puglSetDisplayFunc(self->view, onDisplay);
	puglSetReshapeFunc(self->view, onReshape);
	puglSetKeyboardFunc(self->view, onKeyboard);
	puglSetMotionFunc(self->view, onMotion);
	puglSetMouseFunc(self->view, onMouse);
	puglSetScrollFunc(self->view, onScroll);

	puglCreateWindow(self->view, "dracorex");
	puglSetHandle(self->view, self);
	puglShowWindow(self->view);

	if (resize) {
		resize->ui_resize(resize->handle, self->width, self->height);
	}

	*widget = (void*)puglGetNativeWindow(self->view);

	// Load waveforms for widgets that display them

	FILE *ptr_file;
	int x;
	printf("\n");

	ptr_file =fopen("waves/wave_files.txt", "r");
	fseek(ptr_file, 0L, SEEK_END);
	int length = ftell(ptr_file);
	fseek(ptr_file, 0, SEEK_SET);
	char wave_file_buffer[length];

	int wave_number = 0;

	wavetable_buffer = (float *)malloc(16*4410*sizeof(float));

	int wavetable_length = 4410;

	set_wavetable_buffer(wavetable_buffer, wavetable_length);

	while (fgets(wave_file_buffer,length, ptr_file) != NULL)
	{
		for (int x=0; x<length; x++)
		{
			if (wave_file_buffer[x] == ',')
			{
				wave_file_buffer[x] = 0;
				wave_names[wave_number] = (char*)malloc(x*sizeof(char));
				strcpy(wave_names[wave_number], wave_file_buffer);
				upper_string( wave_names[wave_number] );
			}
		}

		if (wave_file_buffer[0] != 10)
		{
			if (wave_number<16)
		{
			char* filename_and_path = (char*) malloc( strlen(bundle_path) + strlen(wave_file_buffer) + 12);

			sprintf(filename_and_path, "%swaves/%s.wav", bundle_path, wave_file_buffer);
			FILE* wavetable_file = fopen(filename_and_path,"rb");
			fseek(wavetable_file, 80, SEEK_SET);
			fread(&wavetable_buffer[wave_number*wavetable_length] ,1, wavetable_length*sizeof(float), wavetable_file);	
			fclose(wavetable_file);
			printf("%s", filename_and_path);
			printf("\n");
			free(filename_and_path);
		}
				//wavetable_make_wave(wave_number, dracorex_self->bundle_path, wave_file_buffer);
			wave_number++;
		} else break;
	}

	number_of_waves = wave_number;
	set_wave_names(wave_names, number_of_waves);
	printf("\nnumber Of Waves: %d\n", number_of_waves);	

	fclose(ptr_file);

	return self;
}

static int
idle(LV2UI_Handle handle)
{
	dracorexUI* self = (dracorexUI*)handle;

	// Silly pulsing animation to check that idle handler is working
	self->brightness = fmod(self->brightness + 0.01, 1.0);
	puglPostRedisplay(self->view);
	puglProcessEvents(self->view);

	return 0;
}

static void
cleanup(LV2UI_Handle handle)
{

	dracorexUI* self = (dracorexUI*)handle;
	self->exit = true;

	fprintf(stderr, "\ndracorex shutting down........\n");

	puglDestroy(self->view);
	
	for (int x=0; x<number_of_waves; x++)
	{
		free(wave_names[x]);
	}

	free(wavetable_buffer);
	free(self);

}

static void
port_event(LV2UI_Handle handle,
           uint32_t     port_index,
           uint32_t     buffer_size,
           uint32_t     format,
           const void*  buffer)
{
	dracorexUI* self = (dracorexUI*)handle;
	cairo_t* cr = puglGetContext(self->view);
	float value = *(float*)buffer;

	setValue(&self->deliriumUI_window, cr, port_index, value);

	
}

static const LV2UI_Idle_Interface idle_iface = { idle };

static const void*
extension_data(const char* uri)
{
	if (!strcmp(uri, LV2_UI__idleInterface)) {
		return &idle_iface;
	}
	return NULL;
}

static const LV2UI_Descriptor descriptor = {
	dracorex_UI_URI,
	instantiate,
	cleanup,
	port_event,
	extension_data
};

LV2_SYMBOL_EXPORT
const LV2UI_Descriptor*
lv2ui_descriptor(uint32_t index)
{
	switch (index) {
	case 0:
		return &descriptor;
	default:
		return NULL;
	}
}








