#include <raylib.h>
#include <raymath.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#include "gui.h"
#include "util.c"
#include "sprite.h"





void init_gui_system()
{
	DEFAULT_GUI_FONT = LoadFont("437.ttf");
	DEFAULT_STYLE = (STYLE){
		.font = DEFAULT_GUI_FONT,
		.font_size = 16,
		.font_spacing = 1,
		.bg = COLOR_DARKGRAY,
		.fg = WHITE,
		.padding = 10,
		.margin = 0,
		.text_justify = JUSTIFY_LEFT,
		.border_style = BORDER_SOLID,
		.border_color = BLUE,
		.border_size = 2,
		.bg_img = (SPRITE){},
		.border_img = (SPRITE){},
	};


	DEFAULT_STYLE1 = (STYLE){
		.font = DEFAULT_GUI_FONT,
		.font_size = 16,
		.font_spacing = 1,
		.bg = BLACK,
		.fg = WHITE,
		.padding = 10,
		.margin = 0,
		.text_justify = JUSTIFY_LEFT,
		.border_style = BORDER_SOLID,
		.border_color = BLUE,
		.border_size = 2,
		.bg_img = (SPRITE){},
		.border_img = (SPRITE){},
	};
	
	WIDGET_VECTOR_INIT(&__widgets, 40);
	WINDOW_WIDGET = create_widget(0, 0, 0, 0, "", NULL);
	WINDOW_WIDGET->w_name = "WINDOW";

	EMPTY_WIDGET = create_widget(0, 0, 0, 0, "", NULL);
	EMPTY_WIDGET->w_name = "!!EMPTY";
	
	BINDING_MAP_INIT(&WINDOW_WIDGET->binding_map, 20);
	__system_bind_widget(WINDOW_WIDGET, "<MOUSE_BUTTON_LEFT>", __focus_set);

	__WIDGET_FOCUS = WINDOW_WIDGET;
	__WIDGET_ATTENTION = WINDOW_WIDGET;
	__WIDGET_LOCK = EMPTY_WIDGET;
}