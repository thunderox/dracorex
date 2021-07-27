
#include "deliriumUI.h"

void displaydeliriumUIButton(deliriumUI* deliriumUI_window, cairo_t* cr, int widgetNumber)
{
	deliriumUIWidget* deliriumUIWidgets = deliriumUI_window->deliriumUIWidgets;

	int x = deliriumUIWidgets[widgetNumber].x * deliriumUI_window->widgetWidth;
	int y = deliriumUIWidgets[widgetNumber].y * deliriumUI_window->widgetHeight;
	int w = deliriumUIWidgets[widgetNumber].w * deliriumUI_window->widgetWidth;
	int h = deliriumUIWidgets[widgetNumber].h * deliriumUI_window->widgetHeight;

	x += deliriumUIWidgets[widgetNumber].pressed * 2;
	y += deliriumUIWidgets[widgetNumber].pressed * 2;

	cairo_rectangle(cr, x, y, w, h);
	cairo_clip(cr);

	// Draw base
	if (deliriumUIWidgets[widgetNumber].hover) {
		cairo_set_source_rgba(cr, 0,0.2,0, 1);
	} else {
		cairo_set_source_rgba(cr, 0,0.1,0, 1);
	}

	roundedBox(cr, x, y, w, h);
	cairo_fill_preserve(cr);

	// Draw border
	cairo_set_source_rgba(cr, 0.0, 0.0, 0.0, 1);
	cairo_set_line_width(cr, 4.0);
	cairo_stroke(cr);

	// Draw label
	cairo_text_extents_t extents;
	cairo_set_font_size(cr, deliriumUI_window->widgetHeight/2);
	cairo_text_extents(cr, deliriumUIWidgets[widgetNumber].label, &extents);
	cairo_move_to(cr,
	              (x + w / 2) - extents.width / 2,
	              (y + h / 2) + extents.height / 2);

	if (deliriumUIWidgets[widgetNumber].hover) {
		cairo_set_source_rgba(cr, 1, 1, 1, 1);;
	} else {
		cairo_set_source_rgba(cr, 1, 1, 1, 0.6);;
	}

	cairo_show_text(cr, deliriumUIWidgets[widgetNumber].label);

	cairo_reset_clip(cr);
}
