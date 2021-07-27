
#include "deliriumUI.h"
#include <math.h>
#define M_PI 3.14159265358979323846264338327


void setValuedeliriumUIwave_selector(deliriumUI* deliriumUI_window, int widgetNumber, float _value)
{
	deliriumUIWidget* deliriumUIWidgets = deliriumUI_window->deliriumUIWidgets;
	deliriumUIWidgets[widgetNumber].values[0] = (bool)_value;
	deliriumUIWidgets[widgetNumber].pressed = (bool)_value;
}


void displaydeliriumUIWave_Selector(deliriumUI* deliriumUI_window, cairo_t* cr, int widgetNumber)
{
	deliriumUIWidget* deliriumUIWidgets = deliriumUI_window->deliriumUIWidgets;

	float x = deliriumUIWidgets[widgetNumber].x * deliriumUI_window->widgetWidth;
	float y = deliriumUIWidgets[widgetNumber].y * deliriumUI_window->widgetHeight;
	float w = deliriumUIWidgets[widgetNumber].w * deliriumUI_window->widgetWidth;
	float h = deliriumUIWidgets[widgetNumber].h * deliriumUI_window->widgetHeight;


	int current_waveform = deliriumUIWidgets[widgetNumber].values[0];

	float sx = x;
	float sy = y;
	float sw = w;
	float sh = h * 0.8;

	cairo_set_line_width(cr, 4);

	//---------------------

	cairo_set_source_rgb(cr, 0.0, 0.0, 0.0);
	cairo_rectangle(cr, x, y, w, h);
	cairo_clip(cr);

	cairo_set_source_rgb(cr, 0.0, 0.0, 0.0);
	cairo_rectangle(cr, x, y, w, h);
	cairo_stroke(cr);
	cairo_set_source_rgba(cr, 0.0, 0.0, 0.0, 0.25);
	cairo_paint(cr);

	// Draw waveform

	cairo_set_line_width(cr, 1);
	cairo_set_source_rgba(cr, 0.7, 0.7, 0.7, 1);

	cairo_move_to(cr, x+10,y+(h/2));

	int wavetable_steps = (int)(waveform_length)/(w-20);

	cairo_move_to(cr, x+10,y+(h/2)-10);

	for (int bx=0; bx<w-20; bx++)
	{

		cairo_line_to(cr,x+bx+10,y+(h/2)-(wavetable_buffer[(bx*wavetable_steps)+(waveform_length*current_waveform)]*16)-10 );
	}

	cairo_line_to(cr, x+w-10, y+(h/2)-10);
	cairo_stroke(cr);

	cairo_arc(cr, x+10, y+(h/2)-10, (h/20), 0.0, 2 * M_PI);
	cairo_fill(cr);
	cairo_arc(cr, x+w-10, y+(h/2)-10, (h/20), 0.0, 2 * M_PI);
	cairo_fill(cr); 	

	// Draw label

	deliriumUIWidgets[widgetNumber].label = wave_names[current_waveform];

	cairo_text_extents_t extents;
	cairo_set_font_size(cr, h / 5);
	cairo_text_extents(cr, deliriumUIWidgets[widgetNumber].label, &extents);

	cairo_move_to(cr,
	              (x + w / 2) - extents.width / 2,
	              (y + h) - extents.height);

	if (deliriumUIWidgets[widgetNumber].hover) {
		cairo_set_source_rgba(cr, 1, 1, 1, 1);
	} else {
		cairo_set_source_rgba(cr, 1, 1, 1, 0.6);
	}

	cairo_show_text(cr, deliriumUIWidgets[widgetNumber].label);


	cairo_reset_clip(cr);
}
