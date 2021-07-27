
#include "deliriumUI.h"
#include <math.h>
#define M_PI 3.14159265358979323846264338327

void displaydeliriumUIMicroKnob(deliriumUI* deliriumUI_window, cairo_t* cr, int widgetNumber)
{
	deliriumUIWidget* deliriumUIWidgets = deliriumUI_window->deliriumUIWidgets;

	float x = deliriumUIWidgets[widgetNumber].x * deliriumUI_window->widgetWidth;
	float y = deliriumUIWidgets[widgetNumber].y * deliriumUI_window->widgetHeight;
	float w = deliriumUIWidgets[widgetNumber].w * deliriumUI_window->widgetWidth;
	float h = deliriumUIWidgets[widgetNumber].h * deliriumUI_window->widgetHeight;


	float value = deliriumUIWidgets[widgetNumber].values[0]; 

	cairo_set_source_rgb(cr, 0.0, 0.0, 0.0);
	cairo_rectangle(cr, x, y, w, h);
	cairo_clip(cr);

	// Draw label
	cairo_text_extents_t extents;
	cairo_set_font_size(cr, w / 5.5);
	cairo_text_extents(cr, deliriumUIWidgets[widgetNumber].label, &extents);

	cairo_move_to(cr,
	              x + (w/2) - (extents.width/2)  ,
	              (y + h) - (extents.height/3) );

	if (deliriumUIWidgets[widgetNumber].hover) {
		cairo_set_source_rgba(cr, 1, 1, 1, 1);
	} else {
		cairo_set_source_rgba(cr, 1, 1, 1, 0.6);
	}

	cairo_show_text(cr,  deliriumUIWidgets[widgetNumber].label);

	cairo_set_font_size(cr, w / 5);

	 char res[32];

	sprintf(res, "%f",  deliriumUIWidgets[widgetNumber].values[0]);
	res[6] = 0;

	cairo_text_extents(cr, res, &extents);

	cairo_move_to(cr,
	              x + (w/2) - (extents.width/2),
	              y + extents.height);

	cairo_show_text(cr, res );

	float knob_top = y + ( h * (deliriumUIWidgets[widgetNumber].clickTop/100) );
	float knob_height = (h - ( h * (deliriumUIWidgets[widgetNumber].clickBottom/100) ))
		- ( h * (deliriumUIWidgets[widgetNumber].clickTop/100) );

	
	// ------------------------------------------------------------------

	float cos_x =  (w/5) * (cos(((((1+value)*0.75)-0.4)*2) * M_PI));
	float sin_y =  (w/5) * (sin(((((1+value)*0.75)-0.4)*2) * M_PI));

	cairo_pattern_t* pat;

	pat = cairo_pattern_create_radial (x+(w/2) + sin_y, knob_top+(knob_height/2)+cos_x, 0, x+(w/2), knob_top+(knob_height/2) ,(knob_height/2));

	cairo_pattern_add_color_stop_rgba (pat, 0,0.4,0.4,0.4,1);
	cairo_pattern_add_color_stop_rgba (pat, 0.5,0.2,0.2,0.2,1);
	cairo_pattern_add_color_stop_rgba (pat, 0.8,0.17,0.17,0.17,1);
	cairo_pattern_add_color_stop_rgba (pat, 1.0,0.0,0.0,0.0,1);

	cairo_set_source(cr, pat);
	cairo_arc(cr, x+(w/2), knob_top+(knob_height/2), (knob_height/2), 0.0, 2 * M_PI);
	cairo_fill(cr);
	cairo_set_source_rgb(cr, 0.0, 0.0, 0.0);
	cairo_arc(cr, x+(w/2) +cos_x, knob_top+(knob_height/2)+sin_y, (knob_height/12), 0.0, 2 * M_PI);
	cairo_fill(cr);

	cairo_reset_clip(cr);	
	cairo_pattern_destroy (pat);
}










