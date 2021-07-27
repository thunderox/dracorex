 
#include "deliriumUI.h"
#include <math.h>
#define M_PI 3.14159265358979323846264338327


void setValuedeliriumUILFO(deliriumUI* deliriumUI_window, int widgetNumber, float _value)
{
	if (_value<0) _value=0;
	if (_value>1) _value=1;

	deliriumUIWidget* deliriumUIWidgets = deliriumUI_window->deliriumUIWidgets;
	deliriumUIWidgets[widgetNumber].values[0] = _value;
}



//-------------------------------------------------------------------------------

void displaydeliriumUILFO(deliriumUI* deliriumUI_window, cairo_t* cr, int widgetNumber)
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

	cairo_set_source_rgb(cr, 0.0, 0.0, 0.0);
	cairo_rectangle(cr, x, y, w, h);
	cairo_stroke(cr);
	cairo_set_source_rgba(cr, 0.0, 0.0, 0.0, 0.25);
	cairo_paint(cr);

	cairo_text_extents_t extents;

	// -------------------- Draw LFO Speed / BPM 

	char res[32];

	if (deliriumUIWidgets[widgetNumber].hover) {
		cairo_set_source_rgba(cr, 1, 1, 1, 1);
	} else {
		cairo_set_source_rgba(cr, 1, 1, 1, 0.6);
	}


	deliriumUIWidgets[widgetNumber].scaled_value = deliriumUIWidgets[widgetNumber].min + ( deliriumUIWidgets[widgetNumber].values[0]
		* (deliriumUIWidgets[widgetNumber].max-deliriumUIWidgets[widgetNumber].min));

	if ((int)deliriumUIWidgets[widgetNumber].scaled_value < 10)
	{
		sprintf(res, "00%d",  (int)deliriumUIWidgets[widgetNumber].scaled_value);
	}

	if ((int)deliriumUIWidgets[widgetNumber].scaled_value > 9 && (int)deliriumUIWidgets[widgetNumber].scaled_value < 100)
	{
		sprintf(res, "0%d",  (int)deliriumUIWidgets[widgetNumber].scaled_value);
	}

	if ((int)deliriumUIWidgets[widgetNumber].scaled_value > 99)
	{
		sprintf(res, "%d",  (int)deliriumUIWidgets[widgetNumber].scaled_value);
	}

	res[6] = 0;

	cairo_text_extents(cr, res, &extents);
	cairo_set_font_size(cr, w / 3);
	cairo_move_to(cr,
	              x + (w/2) - (extents.width/1.5),
	              y + (h/2));

	cairo_show_text(cr, res );

	// -------------------- Draw label

	cairo_set_font_size(cr, w / 4);
	cairo_text_extents(cr, deliriumUIWidgets[widgetNumber].label, &extents);

	cairo_move_to(cr,
	              x + (w/2) - (extents.width/2),
	              (y + (h/2)) + (extents.height*2) );


	cairo_show_text(cr,  deliriumUIWidgets[widgetNumber].label);

	cairo_reset_clip(cr);

}











