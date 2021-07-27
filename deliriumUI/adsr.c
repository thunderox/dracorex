
#include "deliriumUI.h"
#include <math.h>
#define M_PI 3.14159265358979323846264338327


void setValuedeliriumUIADSR(deliriumUI* deliriumUI_window, int widgetNumber, float _value)
{
	if (_value<0) _value=0;
	if (_value>1) _value=1;

	deliriumUIWidget* deliriumUIWidgets = deliriumUI_window->deliriumUIWidgets;
	int current_value = deliriumUIWidgets[widgetNumber].current_value;

	if (current_value > 0) _value = 1 - _value;

	if (current_value > 3) current_value = 3;

	deliriumUIWidgets[widgetNumber].values[current_value] = _value;
}


//-------------------------------------------------------------------------------

void displaydeliriumUIADSR(deliriumUI* deliriumUI_window, cairo_t* cr, int widgetNumber)
{
	deliriumUIWidget* deliriumUIWidgets = deliriumUI_window->deliriumUIWidgets;

	int x = deliriumUIWidgets[widgetNumber].x * deliriumUI_window->widgetWidth;
	int y = deliriumUIWidgets[widgetNumber].y * deliriumUI_window->widgetHeight;
	int w = deliriumUIWidgets[widgetNumber].w * deliriumUI_window->widgetWidth;
	int h = deliriumUIWidgets[widgetNumber].h * deliriumUI_window->widgetHeight;

	int sx = x;
	int sy = y + (h/6);
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

	// Draw label
	cairo_text_extents_t extents;
	cairo_set_font_size(cr, w / 16);
	cairo_text_extents(cr, deliriumUIWidgets[widgetNumber].label, &extents);

	cairo_move_to(cr,
	              x + (w/2) - (extents.width/2)  ,
	              (y + h) - (extents.height) );

	if (deliriumUIWidgets[widgetNumber].hover) {
		cairo_set_source_rgba(cr, 0.8, 0.8, 0.8, 1);
	} else {
		cairo_set_source_rgba(cr, 0.6, 0.6, 0.6, 1);
	}

	cairo_show_text(cr,  deliriumUIWidgets[widgetNumber].label);

	if ( deliriumUIWidgets[widgetNumber].hover)
	{
	        char res[32];
		sprintf(res, "%f",  deliriumUIWidgets[widgetNumber].values[deliriumUIWidgets[widgetNumber].current_value]);
		cairo_text_extents(cr, res, &extents);

		cairo_move_to(cr,
		              x + (w/2) - (extents.width/2)  ,
		              (y + (h/1.2)) - (extents.height) );

		cairo_show_text(cr,  res);
	}

	cairo_set_line_width(cr, w/80);
	cairo_move_to(cr, sx+(w/5), sy);

	float yp;

	cairo_set_source_rgba(cr, 0.7, 0.7, 0.7, 1);

	for (int xc=1; xc<5; xc++)
	{
		float xcol = xc * (w / 5);
		
		if (xc > 1) yp = (deliriumUIWidgets[widgetNumber].values[xc-1] * (h/2));
			else yp = (h/2) - (deliriumUIWidgets[widgetNumber].values[xc-1] * (h/2));

		if (xc > 1)
		{
			cairo_line_to(cr,  sx+(xcol), sy+yp);
			cairo_stroke(cr);
		}
				
		if (deliriumUIWidgets[widgetNumber].hover && xc==deliriumUIWidgets[widgetNumber].current_value+1 )
		{
			cairo_arc(cr, sx+(xcol), sy+yp, (h/20), 0.0, 2 * M_PI);
		} else
		{
			cairo_arc(cr, sx+(xcol), sy+yp, (h/24), 0.0, 2 * M_PI);
		}

		cairo_fill(cr);


		cairo_move_to(cr, sx+xcol, sy+yp);

	}

	cairo_reset_clip(cr);
}











