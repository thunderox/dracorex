 
#include "deliriumUI.h"
#include <math.h>
#define M_PI 3.14159265358979323846264338327


void setValuedeliriumUIFader(deliriumUI* deliriumUI_window, int widgetNumber, float _value)
{
	if (_value<0) _value=0;
	if (_value>1) _value=1;

	deliriumUIWidget* deliriumUIWidgets = deliriumUI_window->deliriumUIWidgets;
	deliriumUIWidgets[widgetNumber].values[0] = _value;
}



//-------------------------------------------------------------------------------

void displaydeliriumUIFader(deliriumUI* deliriumUI_window, cairo_t* cr, int widgetNumber)
{
	deliriumUIWidget* deliriumUIWidgets = deliriumUI_window->deliriumUIWidgets;

	int x = deliriumUIWidgets[widgetNumber].x * deliriumUI_window->widgetWidth;
	int y = deliriumUIWidgets[widgetNumber].y * deliriumUI_window->widgetHeight;
	int w = deliriumUIWidgets[widgetNumber].w * deliriumUI_window->widgetWidth;
	int h = deliriumUIWidgets[widgetNumber].h * deliriumUI_window->widgetHeight;

	int sx = x;
	int sy = y;
	int sw = w;
	int sh = h * 0.8;

	cairo_set_source_rgb(cr, 0.0, 0.0, 0.0);
	cairo_rectangle(cr, x, y, w, h);
	cairo_clip(cr);

	// Draw label
	cairo_text_extents_t extents;
	cairo_set_font_size(cr, w / 4);
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

	cairo_set_font_size(cr, w / 4);

	 char res[32];

	deliriumUIWidgets[widgetNumber].scaled_value = deliriumUIWidgets[widgetNumber].min + ( deliriumUIWidgets[widgetNumber].values[0]
		* (deliriumUIWidgets[widgetNumber].max-deliriumUIWidgets[widgetNumber].min));

	sprintf(res, "%f",  deliriumUIWidgets[widgetNumber].scaled_value);

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

	float fader_top = y + ( h * (deliriumUIWidgets[widgetNumber].clickTop/100) );
	float fader_height = (h - ( h * (deliriumUIWidgets[widgetNumber].clickBottom/100) ))
		- ( h * (deliriumUIWidgets[widgetNumber].clickTop/100) );

	float value = 1 - deliriumUIWidgets[widgetNumber].values[0];

	float value_to_ypixel = (value * fader_height) ;



	// ------------------------------------------------------------------

	// draw vertical scale bars
	cairo_set_line_width(cr, 0.4);
	cairo_set_source_rgba(cr, 0.9,0.9,0.9,1.0);

	for (int yl=0; yl<fader_height; yl+=(fader_height/10))
	{
		cairo_move_to(cr, x+(w/3), fader_top+yl);
	 	cairo_line_to(cr, x+(w-(w/3)), fader_top+yl);
	    	cairo_stroke(cr);
	}

	// draw vertical grey line down the middle
	cairo_set_line_width(cr, 4);
	cairo_set_source_rgb(cr, 0.0, 0.0, 0.0);
	cairo_move_to(cr, x+(w/2), fader_top);
	cairo_line_to(cr, x+(w/2), fader_top + fader_height);
	cairo_stroke(cr);

	// draw horizontal thin black line at fader_value height
	cairo_set_line_width(cr, fader_height/6);
	cairo_set_source_rgba(cr, 0.0,0.0,0.0,1.0);
	cairo_move_to(cr, x+(w/4), fader_top+value_to_ypixel);
	cairo_line_to(cr, x+(w-(w/4)), fader_top+value_to_ypixel);
	cairo_stroke(cr);

	// draw horizontal thick black line at fader_value height
	cairo_set_line_width(cr, fader_height/4);
	cairo_set_source_rgba(cr, 0.0,0.0,0.0,0.3);
	cairo_move_to(cr, x+(w/4), fader_top+value_to_ypixel);
	cairo_line_to(cr, x+(w-(w/4)), fader_top+value_to_ypixel + 1.1);
	cairo_stroke(cr);

	// set up grad
	cairo_pattern_t* pat;

	pat = cairo_pattern_create_linear(x+(w/4), fader_top+value_to_ypixel, x+(w-(w/4)), fader_top+value_to_ypixel );
	cairo_pattern_add_color_stop_rgba(pat, 0.0,0.2,0.2,0.2,1);
        cairo_pattern_add_color_stop_rgba(pat, 0.4,0.8,0.8,0.8,1);
        cairo_pattern_add_color_stop_rgba(pat, 1.0,0.4,0.4,0.4,1);

	// draw horizontal blue line at fader_value height
	cairo_set_line_width(cr, h/24);
	cairo_set_source(cr, pat);
	cairo_move_to(cr, x+(w/4), fader_top+value_to_ypixel);
	cairo_line_to(cr, x+ (w-(w/4)), fader_top+value_to_ypixel);
	cairo_stroke(cr);

	cairo_reset_clip(cr);
	cairo_pattern_destroy (pat);
}











