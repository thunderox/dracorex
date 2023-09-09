
#include "delirium_ui.hpp"
#include <sstream>
#include<bits/stdc++.h> 

//-------------------------------------------------------------------------------------------


void Delirium_UI_Widget_Selector::Draw(cairo_t* cr)
{

	if (integer) values[current_value] = int(values[current_value]);
	Convert_Scaled_To_Value();

	float x = x_position * x_grid_size;
	float y = y_position * y_grid_size;
	float w = width * x_grid_size;
	float h = height * y_grid_size;

	cairo_set_source(cr, theme_background_grad);
	cairo_rectangle(cr, x, y, w, h);
	cairo_fill(cr);
	
	cairo_set_source_rgb(cr, 0.15,0,0);
	cairo_rectangle(cr, x, y + (font_size), w, h - (font_size * 3) );
	cairo_fill(cr);
	
	cairo_text_extents_t extents;
	cairo_set_font_size(cr, font_size);
	float x_text_centred; 

	// SHOW CURRENT WAVEFORM
	
	if (hover)
	{
		cairo_set_source_rgb(cr, 1, 1, 1);
	} else {
		cairo_set_source_rgb(cr, 0.6,0.6,0.6);
	}
	
	string name = wavetables[values[current_value]].name ;
	transform(name.begin(), name.end(), name.begin(), ::toupper);
	name = name.substr(0,name.size()-4);

	cairo_text_extents(cr, name.c_str(), &extents);
	x_text_centred = (x + w / 2) - extents.width / 2;
	cairo_move_to(cr,x_text_centred, y+h-(font_size/2));
	cairo_show_text(cr, name.c_str());
				
	if (hover)
	{
		cairo_set_source_rgb(cr, 0.7,0.7,0.7);
	} else {
		cairo_set_source_rgb(cr, 0.5,0.5,0.5);
	}
	
	// DRAW WAVEFORM
	
	long waveform_length = wavetables[values[current_value]].length;
	int wavetable_steps = (long)(waveform_length)/(w-20);
	
	cairo_move_to(cr, x+10,y+(h/2)-10);		

	for (int bx=0; bx<w-20; bx++)
	{

		cairo_line_to(cr,x+bx+10,y+(h/2)-(wavetables[values[current_value]].buffer[(bx*wavetable_steps)]*(h/5))-10 );
	}

	cairo_line_to(cr, x+w-10, y+(h/2)-10);
	cairo_stroke(cr);
	
	// DRAW LABEL
	
	cairo_set_source_rgb(cr, 0,0,0);
	cairo_rectangle(cr, x, y, w,font_size);
	cairo_fill(cr);

	if (hover)
	{
		cairo_set_source_rgba(cr, 1, 1, 1, 1);
	} else {
		cairo_set_source_rgba(cr, 1, 1, 1, 0.6);

	}

	cairo_text_extents(cr, label.c_str(), &extents);
	x_text_centred = (x + w / 2) - extents.width / 2;
	cairo_move_to(cr,x_text_centred, y+font_size);
	cairo_show_text(cr, label.c_str());



}

//-------------------------------------------------------------------------------------------

void Delirium_UI_Widget_Selector::Left_Button_Press(int xm, int ym)
{

	float x = x_position * x_grid_size;
	float y = y_position * y_grid_size;
	float w = width * x_grid_size;
	float h = height * y_grid_size;
		
	toggle_mode = 1 - toggle_mode;

	if (!toggle_mode)
	{
		values[current_value]++;
		if (values[current_value] > max) values[current_value]=0;
	}
	
	
	Convert_Scaled_To_Value();
	
}


//----------------------------------------------------------------------------------------------------------------------------------------
// MOUSE OVER

void Delirium_UI_Widget_Selector::Mouse_Over(int xm, int ym)
{
}



