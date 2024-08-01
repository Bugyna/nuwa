#include <raylib.h>
#include <raymath.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <dirent.h> 

#include "util.c"
#include "sprite.h"
#include "SHM/hashmap.h"
#include "arena.h"
#include "eventloop.h"

#ifndef GUI_H_
#define GUI_H_

enum
{
	W_WINDOW=0,
	W_FRAME,
	W_LABEL,
	W_BUTTON,
	W_TEXT_INPUT,
};

typedef struct STYLE STYLE;
typedef struct WIDGET WIDGET;

typedef struct BINDING BINDING;



#define BIND_FN_PARAMS WIDGET* w, EVENT e
// typedef struct W_LABEL W_LABEL;
// typedef struct W_BUTTON W_BUTTON;
// typedef struct W_TEXT_INPUT W_TEXT_INPUT;

#define COLOR_DARKGRAY DARKGRAY


typedef enum
{
	BORDER_NONE=0,
	BORDER_RIDGE,
	BORDER_SOLID,
	BORDER_GROOVE,
	BORDER_CUSTOM,
} BORDER_STYLE;


typedef enum
{
	JUSTIFY_LEFT,
	JUSTIFY_CENTER,
	JUSTIFY_RIGHT,
} TEXT_JUSTIFY;


struct STYLE
{
	Font font;
	int font_size, font_spacing;
	Color bg, fg;

	int padding;
	int margin;

	TEXT_JUSTIFY text_justify;
	
	BORDER_STYLE border_style;
	Color border_color;
	int border_size;

	SPRITE bg_img;
	SPRITE border_img;
	
	
};


Font DEFAULT_GUI_FONT;
STYLE DEFAULT_STYLE;
STYLE DEFAULT_STYLE1;


struct BINDING
{
	// int keys[12];
	// int buttons[6];
	char* str;

	int (*system) (BIND_FN_PARAMS);
	int (*custom) (BIND_FN_PARAMS);
};

DEFINE_HASHMAP(BINDING_MAP, BINDING)

struct WIDGET
{
	
	uint8_t type;
	char* type_name;
	char* w_name;

	STYLE* style;
	
	const char* content_text;
	Rectangle pos;

	BINDING_MAP binding_map;

	
	// union 
	// {
		// W_LABEL label;
		// W_BUTTON button;
		// W_TEXT_INPUT text_input;
	// };
	
};

WIDGET __WINDOW_WIDGET = {
	.type = W_WINDOW,
	.type_name = "WINDOW",
	.w_name = "BASE_WINDOW",

	.style = &DEFAULT_STYLE,
	.content_text = "",
	.pos = (Rectangle){.x=0, .y=0, .width=0, .height=0},
	
};

// BINDING_MAP_INIT(&__WINDOW_WIDGET->binding_map, 50);

// WIDGET* __WIDGET_FOCUS = NULL;
// WIDGET* __WIDGET_ATTENTION = NULL;
// WIDGET* __WIDGET_LOCK = NULL;

WIDGET* WINDOW_WIDGET;
WIDGET* EMPTY_WIDGET;
WIDGET* __WIDGET_FOCUS = &__WINDOW_WIDGET;
WIDGET* __WIDGET_ATTENTION = &__WINDOW_WIDGET;
WIDGET* __WIDGET_LOCK = &__WINDOW_WIDGET;
DEFINE_HASHMAP(WIDGET_MAP, WIDGET)


DEFINE_VECTOR(WIDGET_VECTOR, WIDGET)

WIDGET_VECTOR __widgets;
// WIDGET create_label(int w, int h)
// {
	// WIDGET w;
	// w.label = 
	// return w;
// }


// void change_label_text(WIDGET* label, const char* text)
// {
	// label->content_text = text;
// }


void init_gui_system();

// char** get_bind_names_from_key(const char* bind)
// {
	// char** ret = calloc(10, sizeof(char*));
	// char* tmp = malloc(30);
	
	// int len = strlen(bind), size = 0, offset = 0, i = 0;

	// bool in_special = false;
	// // SDL_Log("len: %d", len);
	// for ( ; offset < len; offset++)
	// {
		// tmp[size] = '\0';
		// // SDL_Log("tmp: %s %d	", tmp, offset);

		// if (!in_special && bind[offset] == '<') {
			// tmp[size++] = bind[offset];
			// in_special = true;
		// }

		// else if (in_special && bind[offset] == '>') {
			// tmp[size++] = bind[offset];
			// tmp[size++] = '\0';
			// ret[i] = malloc(size);
			// strcpy(ret[i++], tmp);
			// // SDL_Log("tmp end: %s", tmp);
			// in_special = false;
			// size = 0;
		// }

		// else if (in_special && size) tmp[size++] = bind[offset];

		// else {
			// // SDL_Log("dwaere %c %d", bind[offset], offset);
			// ret[i] = malloc(2);
			// tmp[0] = bind[offset];
			// tmp[1] = '\0';
			// strcpy(ret[i++], tmp);
		// }
	// }


	// free(tmp);
	// return ret;
// }

// BINDING* parse_keybind(const char* keybind, int (*func) (BIND_FN_PARAMS))
// {
	// BINDING* b = calloc(1, sizeof(BINDING));
	// strcpy(b->str, keybind);
	// b->custom = func;
	// return b;
// }

void bind_widget(WIDGET* w, char* keybind, int (*func) (BIND_FN_PARAMS))
{
	BINDING* b = calloc(1, sizeof(BINDING));
	b->str = malloc(strlen(keybind));
	strcpy(b->str, keybind);
	b->custom = func;
	BINDING_MAP_ADD(&w->binding_map, keybind, b);
}


void __system_bind_widget(WIDGET* w, const char* keybind, int (*func) (BIND_FN_PARAMS))
{
	// BINDING* b = malloc(sizeof(BINDING));
	BINDING* b = calloc(1, sizeof(BINDING));
	
	b->str = malloc(strlen(keybind));
	strcpy(b->str, keybind);
	b->system = func;
	BINDING_MAP_ADD(&w->binding_map, b->str, b);
}


int execute_widget_bind(WIDGET* w, char* keybind, EVENT e)
{
	BINDING* b = BINDING_MAP_GET(&w->binding_map, keybind);
	int ret = 0;
	if (b == NULL) return ret;
	// if (b->custom == NULL && b->system == NULL) return;
	if (b->custom != NULL) ret = b->custom(w, e);
	if (b->system != NULL) ret = b->system(w, e);
	return ret;
}

int __focus_set(BIND_FN_PARAMS)
{
	__WIDGET_FOCUS = w;
	return 1;
}

int focus_set(WIDGET* w)
{
	__WIDGET_FOCUS = w;
}

WIDGET* focus_get()
{
	return __WIDGET_FOCUS;
}





int change_style(BIND_FN_PARAMS)
{
	if (w->style == &DEFAULT_STYLE)
		w->style = &DEFAULT_STYLE1;
	else w->style = &DEFAULT_STYLE;

	printf("changed style\n");
}

int test_drag(BIND_FN_PARAMS)
{
	Vector2 mouse_pos = e.mouse_pos;
	Vector2 rel_pos = get_relative_pos_PointRec(GetMousePosition(), w->pos);
	printf("pos: %f,%f | %f,%f == %f\n", mouse_pos.x, mouse_pos.y, rel_pos.x, rel_pos.y, mouse_pos.x - rel_pos.x);
	// w->pos.x = mouse_pos.x - rel_pos.x;
	// w->pos.y = mouse_pos.y - rel_pos.y;
	w->pos.x += e.mouse_delta.x;
	w->pos.y += e.mouse_delta.y;
	// w->pos.x += e.mouse_delta_rel.x;
	// w->pos.y += e.mouse_delta_rel.y;
	return 1;
}

int test_drag_x(BIND_FN_PARAMS)
{
	Vector2 mouse_pos = e.mouse_pos;
	Vector2 rel_pos = get_relative_pos_PointRec(GetMousePosition(), w->pos);
	// w->pos.x = mouse_pos.x - rel_pos.x;
	// w->pos.y = mouse_pos.y - rel_pos.y;
	w->pos.x += e.mouse_delta.x;
	// w->pos.x += e.mouse_delta_rel.x;
	// w->pos.y += e.mouse_delta_rel.y;
	return 1;
}

int test_drag_y(BIND_FN_PARAMS)
{
	Vector2 mouse_pos = e.mouse_pos;
	Vector2 rel_pos = get_relative_pos_PointRec(GetMousePosition(), w->pos);
	// w->pos.x = mouse_pos.x - rel_pos.x;
	// w->pos.y = mouse_pos.y - rel_pos.y;
	w->pos.y += e.mouse_delta.y;
	// w->pos.x += e.mouse_delta_rel.x;
	// w->pos.y += e.mouse_delta_rel.y;
	return 1;
}

int test_resize(BIND_FN_PARAMS)
{
	printf("resize\n");

	if (e.mouse_wheel_move > 0) {
		w->pos.width += 10;
		w->pos.height += 10;
	}

	else {
		w->pos.width -= 10;
		w->pos.height -= 10;
	}
	return 1;
}

WIDGET* create_widget(int x, int y, int width, int height, const char* text, STYLE* style)
{
	// WIDGET* w = calloc(1, sizeof(WIDGET));
	WIDGET ww;
	WIDGET* w = &ww;
	w->w_name = "W__WIDGET";
	// w->binding_map = calloc(1, sizeof(BINDING_MAP));
	BINDING_MAP_INIT(&w->binding_map, 50);
	w->pos = (Rectangle){
		.x = x, .y = y,
		.width = width, .height = height
	};

	w->content_text = text;

	if (style == NULL) style = &DEFAULT_STYLE;
	w->style = style;

	__system_bind_widget(w, "<MOUSE_BUTTON_LEFT>", __focus_set);
	bind_widget(w, "<MOUSE_BUTTON_LEFT><MOUSE_MOVE>", test_drag);
	bind_widget(w, "<MOUSE_BUTTON_RIGHT><MOUSE_MOVE>", test_drag_x);
	bind_widget(w, "<KEY_LEFT_SHIFT><MOUSE_BUTTON_RIGHT><MOUSE_MOVE>", test_drag_y);
	bind_widget(w, "[KEY_A]", change_style);
	bind_widget(w, "<KEY_A><MOUSE_MOVE>", change_style);
	bind_widget(w, "<MOUSE_WHEEL_MOVE>", test_resize);

	w = WIDGET_VECTOR_ADD(&__widgets, ww);
	return w;
}


void draw_widget(WIDGET* w)
{
	DrawRectangleRec(w->pos, w->style->bg);
	// DrawRectangle(
		// w->pos.x,
		// w->pos.y,
		// w->pos.width,
		// w->pos.height,
		// w->style->bg
	// );
	
	DrawTextEx(w->style->font, w->content_text, (Vector2){w->pos.x+w->style->padding, w->pos.y+w->style->padding}, w->style->font_size, w->style->font_spacing, w->style->fg);
	if (w->style->border_style == BORDER_SOLID)
		DrawRectangleLinesEx(w->pos, w->style->border_size, RED);
}



#endif