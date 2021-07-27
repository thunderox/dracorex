
#include "deliriumUI.h"
#include <math.h>
#define M_PI 3.14159265358979323846264338327

void displaydeliriumUIKnob(deliriumUI* deliriumUI_window, cairo_t* cr, int widgetNumber)
{
	deliriumUIWidget* deliriumUIWidgets = deliriumUI_window->deliriumUIWidgets;

	int x = deliriumUIWidgets[widgetNumber].x * deliriumUI_window->widgetWidth;
	int y = deliriumUIWidgets[widgetNumber].y * deliriumUI_window->widgetHeight;
	int w = deliriumUIWidgets[widgetNumber].w * deliriumUI_window->widgetWidth;
	int h = deliriumUIWidgets[widgetNumber].h * deliriumUI_window->widgetHeight;

	float value = deliriumUIWidgets[widgetNumber].values[0];  // deliriumUIWidgets[widgetNumber].value;

	float cos_x =  (w/5) * (cos(((((1-value)*0.75)-0.3)*2) * M_PI));
	float sin_y =  (w/5) * (sin(((((1-value)*0.75)-0.3)*2) * M_PI));

	cairo_set_source_rgb(cr, 0.0, 0.0, 0.0);
	cairo_rectangle(cr, x, y, w, h);
	cairo_clip(cr);

	// draw outer level scale lines

	cairo_set_source_rgba(cr, 1.0,1.0,1.0,0.2);
	cairo_set_line_width(cr, h/18);


	for (float scale=-.05; scale<1; scale+=0.05 )
	{
 		float cos_x =  (h/3) * (cos(((((1-scale)*0.75)-0.67)*2) * M_PI));
		float sin_y =  (h/3) * (sin(((((1-scale)*0.75)-0.67)*2) * M_PI));
 		float cos_x2 =  (h/2.6) * (cos(((((1-scale)*0.75)-0.67)*2) * M_PI));
		float sin_y2 =  (h/2.6) * (sin(((((1-scale)*0.75)-0.67)*2) * M_PI));

		if (scale > 0.9-value && value != 0) { cairo_set_source_rgba(cr, 1.0,1.0,1.0,0.7); }

		cairo_move_to(cr,  x + (w/2) + cos_x, y + (h/2.2) + sin_y);
		cairo_line_to(cr,  x + (w/2) + cos_x2, y + (h/2.2) + sin_y2);
		cairo_stroke(cr);
	} 


	
	cos_x =  (h/5) * (cos((((value*0.75)-0.61)*2) * M_PI));
	sin_y =  (h/5) * (sin((((value*0.75)-0.61)*2) * M_PI));

	cairo_set_source_rgb(cr, 0.2,0.2,0.2);
	cairo_arc(cr, x + (w/2), y + (h/2.2), (h/3.4), 0.0, 2 * M_PI);
	cairo_fill(cr);
	cairo_set_source_rgb(cr, 0.0, 0.0, 0.0);
	cairo_arc(cr, x + cos_x + (w/2), y + sin_y + (h/2.2), (h/20), 0.0, 2 * M_PI);
	cairo_fill(cr);

	cairo_stroke(cr);


	// Draw label
	cairo_text_extents_t extents;
	cairo_set_font_size(cr, h / 6);
	cairo_text_extents(cr, deliriumUIWidgets[widgetNumber].label, &extents);

	cairo_move_to(cr,
	              (x + w / 2) - extents.width / 2,
	              (y + h) - extents.height / 4);

	if (deliriumUIWidgets[widgetNumber].hover) {
		cairo_set_source_rgba(cr, 1, 1, 1, 1);;
	} else {
		cairo_set_source_rgba(cr, 1, 1, 1, 0.6);;
	}

	cairo_show_text(cr, deliriumUIWidgets[widgetNumber].label);

	cairo_reset_clip(cr);
}
