#include <raylib.h>
#include <raymath.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#include "gui.h"
#include "util.c"
#include "sprite.h"


void init_gui_system(int width, int height)
{
	DEFAULT_GUI_FONT = LoadFont("437.ttf");
	
	DEFAULT_FRAME_STYLE = (STYLE){
		.font = DEFAULT_GUI_FONT,
		.font_size = 16,
		.font_spacing = 1,
		.bg = WHITE,
		.fg = BLACK,
		.padding = 0,
		.margin = 0,
		.text_justify = JUSTIFY_LEFT,
		.border_style = BORDER_SOLID,
		.border_color = MAGENTA,
		.border_size = 2,
		.bg_img = (SPRITE){},
		.border_img = (SPRITE){},
	};

	DEFAULT_STYLE = (STYLE){
		.font = DEFAULT_GUI_FONT,
		.font_size = 16,
		.font_spacing = 1,
		.bg = WHITE,
		.fg = BLACK,
		.padding = 3,
		.margin = 0,
		.text_justify = JUSTIFY_LEFT,
		.border_style = BORDER_SOLID,
		.border_color = BLACK,
		.border_size = 1,
		.bg_img = (SPRITE){},
		.border_img = (SPRITE){},
	};

	DEFAULT_STYLE1 = (STYLE){
		.font = DEFAULT_GUI_FONT,
		.font_size = 16,
		.font_spacing = 1,
		.bg = DARKGRAY,
		.fg = WHITE,
		.padding = 10,
		.margin = 0,
		.text_justify = JUSTIFY_CENTER,
		.border_style = BORDER_SOLID,
		.border_color = BLUE,
		.border_size = 1,
		.bg_img = (SPRITE){},
		.border_img = (SPRITE){},
	};


	DEFAULT_STYLE2 = (STYLE){
		.font = DEFAULT_GUI_FONT,
		.font_size = 16,
		.font_spacing = 1,
		.bg = BLACK,
		.fg = WHITE,
		.padding = 10,
		.margin = 0,
		.text_justify = JUSTIFY_RIGHT,
		.border_style = BORDER_SOLID,
		.border_color = BLUE,
		.border_size = 1,
		.bg_img = (SPRITE){},
		.border_img = (SPRITE){},
	};
	
	WIDGET_VECTOR_INIT(&__widgets, 40);
	WIDGET_PTR_VECTOR_INIT(&WIDGET_RENDER_QUEUE, 40);
	
	WINDOW_WIDGET = create_widget(0, 0, W_WINDOW, width, height, "", NULL);
	WINDOW_WIDGET->w_name = "WINDOW";

	EMPTY_WIDGET = create_widget(0, 0, W_WINDOW, 0, 0, "", NULL);
	EMPTY_WIDGET->w_name = "!!EMPTY";

	
	__system_bind_widget(WINDOW_WIDGET, "<MOUSE_BUTTON_LEFT>", __focus_set);
	__system_bind_widget(WINDOW_WIDGET, "<WINDOW_RESIZED>", __window_handle_resize);

	__WIDGET_FOCUS = WINDOW_WIDGET;
	__WIDGET_ATTENTION = WINDOW_WIDGET;
	__WIDGET_LOCK = EMPTY_WIDGET;
}
