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
#include "str_vec.h"
#include "eventloop.h"

#ifndef GUI_H_
#define GUI_H_


SPRITE EMPTY_SPRITE = (SPRITE){0};
SPRITE* EMPTY_SPRITE_PTR = &EMPTY_SPRITE;

typedef enum
{
	W_WINDOW=0,
	W_FRAME,
	W_LABEL,
	W_BUTTON,
	W_TEXT_INPUT,
	W_NUMBER_INPUT,
	W_SLIDER,
	W_DROPDOWN,
} WIDGET_TYPE;



#define __STRINGIFY_SWITCH_CASE(N) case N: return #N;


const char* widget_type_as_str(WIDGET_TYPE type)
{
	switch (type)
	{
		__STRINGIFY_SWITCH_CASE(W_WINDOW);
		__STRINGIFY_SWITCH_CASE(W_FRAME);
		__STRINGIFY_SWITCH_CASE(W_LABEL);
		__STRINGIFY_SWITCH_CASE(W_BUTTON);
		__STRINGIFY_SWITCH_CASE(W_TEXT_INPUT);
		__STRINGIFY_SWITCH_CASE(W_DROPDOWN);
	}
}

typedef struct STYLE STYLE;
typedef struct GRID GRID;
typedef struct WIDGET WIDGET;

typedef struct BINDING BINDING;

typedef struct CONTENT_SIZE
{
	float width, height;
	float rows, columns;
} CONTENT_SIZE;

#define BIND_FN_PARAMS WIDGET* w, EVENT e
#define EVENT_FUNCTION_DEF int (*func) (BIND_FN_PARAMS)

// typedef int (*func) (BIND_FN_PARAMS) EVENT_FUNCTION_DEF;
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


typedef enum
{
	GRID_FILL=-1,
	GRID_AUTO=-2,
	GRID_=-3,
} GRID_PARAMS;

struct GRID
{
	int row_count;
	int cell_count;

	int max_row_width;
	int max_row_height;

	int row_width;
	int row_height;

	int max_cell_width;
	int max_cell_height;
	
	int cell_width;
	int cell_height;

	int min_cell_width;
	int min_cell_height;
};

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


	// Positioning
	bool use_grid;
	GRID grid;
	
	
};

Font DEFAULT_GUI_FONT;
STYLE DEFAULT_FRAME_STYLE;
STYLE DEFAULT_FRAME_STYLE1;
STYLE DEFAULT_STYLE;
STYLE DEFAULT_STYLE1;
STYLE DEFAULT_STYLE2;


struct BINDING
{
	// int keys[12];
	// int buttons[6];
	char* str;

	int (*system) (BIND_FN_PARAMS);
	int (*custom) (BIND_FN_PARAMS);
};

DEFINE_HASHMAP(BINDING_MAP, BINDING)


DEFINE_VECTOR(WIDGET_PTR_VECTOR, WIDGET*)
DEFINE_VECTOR(STRING_VECTOR, STRING)
DEFINE_VECTOR(STRING_PTR_VECTOR, STRING*)


struct WIDGET
{
	WIDGET* __parent;
	WIDGET_PTR_VECTOR __children;
	bool __draw_children;
	bool disabled;

	WIDGET_TYPE type;
	char* type_name;
	char* w_name;

	STYLE* style;

	STRING text;
	STRING_VECTOR lines;
	Vector2 cursor;
	
	Rectangle pos;
	Rectangle vw;

	BINDING_MAP binding_map;

	SPRITE* img;

	
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
	.text = (STRING){.str="", .index=0, .available=0},
	.pos = (Rectangle){.x=0, .y=0, .width=0, .height=0},
	.img = &EMPTY_SPRITE,
	
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
WIDGET* __WIDGET_LOCK1 = &__WINDOW_WIDGET;
DEFINE_HASHMAP(WIDGET_MAP, WIDGET)


DEFINE_VECTOR(WIDGET_VECTOR, WIDGET)

WIDGET_VECTOR __widgets;
WIDGET_PTR_VECTOR WIDGET_RENDER_QUEUE;


CONTENT_SIZE __get_content_size(WIDGET* parent)
{
	int width, height;
	float _c = -1, _cc = -1, _r = -1;
	int x_offset = 0, y_offset = 0;

	if (parent->style->grid.row_width == GRID_AUTO) {
		width = parent->pos.width / parent->__children.index;
		
		if (width < parent->style->grid.min_cell_width) {
			width = parent->style->grid.min_cell_width;
		}

		else if (width > parent->style->grid.max_cell_width) {
			width = parent->style->grid.max_cell_width;
		}
	}

	else {
		width = parent->style->grid.row_width / parent->__children.index;
	}

	_c = floor(parent->pos.width / width);

	_r = ceil(parent->__children.index / _c);
	// printf("r: %f %f\n", _r, _c);


	if (parent->style->grid.row_height == GRID_AUTO) {
		height = parent->pos.height / _r;
		if (height < parent->style->grid.min_cell_width) {
			height = parent->style->grid.min_cell_height;
		}
		else if (height > parent->style->grid.max_cell_height) {
			height = parent->style->grid.max_cell_height;
		}
		
	}

	else
		height = parent->style->grid.row_height;

	return (CONTENT_SIZE){.width=width, .height=height, .rows=_r, .columns=_c};
}


Vector2 __get_grid_row_count(WIDGET* parent)
{
	
}

Vector2 __get_row_column_count(WIDGET* parent)
{
	
}

Vector2 __get_grid_element_size(WIDGET* parent)
{
	
}
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

		// else if (in_specia && size) tmp[size++] = bind[offset];

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
	BINDING* b = NULL;
	b = BINDING_MAP_GET(&w->binding_map, keybind);

	if (b != NULL) {
		b->custom = func;
	}

	else {
		b = calloc(1, sizeof(BINDING));
		b->str = malloc(strlen(keybind));
		strcpy(b->str, keybind);
		b->custom = func;
		BINDING_MAP_ADD(&w->binding_map, b->str, b);
	}
}


void __system_bind_widget(WIDGET* w, const char* keybind, int (*func) (BIND_FN_PARAMS))
{
	// BINDING* b = malloc(sizeof(BINDING));
	BINDING* b = NULL;
	b = BINDING_MAP_GET(&w->binding_map, keybind);

	if (b != NULL) {
		b->system = func;
	}

	else {
		b = calloc(1, sizeof(BINDING));
		b->str = malloc(strlen(keybind));
		strcpy(b->str, keybind);
		b->system = func;
		BINDING_MAP_ADD(&w->binding_map, b->str, b);
	}
}

void unbind_widget(WIDGET* w, const char* keybind)
{
	BINDING* b = NULL;
	b = BINDING_MAP_GET(&w->binding_map, keybind);

	if (b != NULL) {
		b->custom = NULL;
	}
}

void __system_unbind_widget(WIDGET* w, const char* keybind)
{
	BINDING* b = NULL;
	b = BINDING_MAP_GET(&w->binding_map, keybind);

	if (b != NULL) {
		b->system = NULL;
	}
}


int execute_widget_bind(WIDGET* w, char* keybind, EVENT e)
{
	if (w->disabled) return -1;
	if (w->__parent != NULL) execute_widget_bind(w->__parent, keybind, e);
	BINDING* b = BINDING_MAP_GET(&w->binding_map, keybind);
	int ret = 0;
	if (b == NULL) return ret;
	// printf("executing: %s %p\n", keybind, b->custom);
	// if (b->custom == NULL && b->system == NULL) return;
	if (b->custom != NULL) ret = b->custom(w, e);
	if (!ret && b->system != NULL) ret = b->system(w, e);
	return ret;
}


int __window_handle_resize(BIND_FN_PARAMS)
{
	// int width = GetRenderWidth();
	// int height = GetRenderHeight();
	int width = GetScreenWidth();
	int height = GetScreenHeight();

	w->pos.width = width;
	w->pos.height = height;

	printf("RESIZED: %d %d\n", width, height);
	return 0;
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

	else if (w->style == &DEFAULT_STYLE1)
		w->style = &DEFAULT_STYLE2;

	else w->style = &DEFAULT_STYLE;

	printf("changed style\n");
}

int test_drag(BIND_FN_PARAMS)
{
	Vector2 mouse_pos = e.mouse_pos;
	Vector2 rel_pos = get_relative_pos_PointRec(GetMousePosition(), w->pos);
	// printf("pos: %f,%f | %f,%f == %f\n", mouse_pos.x, mouse_pos.y, rel_pos.x, rel_pos.y, mouse_pos.x - rel_pos.x);
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


int scroll_x(BIND_FN_PARAMS)
{
	CONTENT_SIZE size = __get_content_size(w);
	int offset = size.width / 2;
	if (e.mouse_wheel_move > 0) {
		if (w->vw.x > 0) w->vw.x += offset;
	}

	else {
		if (w->vw.x < w->vw.width && w->vw.x < size.width * size.columns) w->vw.x += offset;
	}
	printf("viewport: %f.%f\n", w->vw.x, w->vw.y);

	return 1;
}

int scroll_y(BIND_FN_PARAMS)
{
	CONTENT_SIZE size = __get_content_size(w);
	int offset = size.height / 2;
	
	if (e.mouse_wheel_move > 0) {
		if (w->vw.y > 0) w->vw.y -= offset;
	}

	else {
		float vw_rows = w->vw.height / size.height;

		if (w->vw.y < (size.rows - vw_rows)*size.height) w->vw.y += offset;
	}
	printf("viewport: %f   %f.%f %f %f %f \n", w->pos.height, w->vw.y, size.height, size.rows, size.columns, (size.height * size.rows));

	return 1;
}

int resize_x(BIND_FN_PARAMS)
{
	if (e.mouse_wheel_move > 0) {
		w->pos.width += 10;
	}

	else {
		w->pos.width -= 10;
	}
	return 1;
}

int resize_edge(BIND_FN_PARAMS)
{
	bool resize_x = false;
	bool resize_y = false;
	bool fx = false;
	bool fy = false;

	int limit = 30;

	printf("DRAG START POS: %f.%f\n", e.drag_pos.x, e.drag_pos.y);
	// e.mouse_pos = GetMousePosition();
	// printf("here %f %f\n", e.last_mouse_pos.x, w->pos.x+w->pos.width);
	
	// if (w != __WIDGET_LOCK && w == __WIDGET_ATTENTION) return 0;
	if (w != __WIDGET_LOCK1 && e.drag_pos.x > w->pos.x + limit && e.drag_pos.x < w->pos.x+w->pos.width - limit) { printf("returning\n"); return 0; }

	// if (w != __WIDGET_LOCK) limit = 15;

	// if (w != __WIDGET_LOCK) {
		if (e.mouse_pos.x >= w->pos.x - limit && e.mouse_pos.x <= w->pos.x + limit) resize_x = true;
		else if (e.mouse_pos.x > w->pos.x + w->pos.width - limit && e.mouse_pos.x <= w->pos.x + w->pos.width + limit) {resize_x = true; fx = true;}
		
		if (e.mouse_pos.y >= w->pos.y - limit && e.mouse_pos.y <= w->pos.y + limit) resize_y = true;
		else if (e.mouse_pos.y >= w->pos.y + w->pos.height - limit && e.mouse_pos.y <= w->pos.y + w->pos.height + limit) { resize_y = true; fy = true; }
	// }

	// else {
		// if (e.mouse_pos.x <= w->pos.x) resize_x = true;
		// else if (e.mouse_pos.x >= w->pos.x) {resize_x = true; fx = true;}
		
		// if (e.mouse_pos.y <= w->pos.y) resize_y = true;
		// else if (e.mouse_pos.y >= w->pos.y + w->pos.height) { resize_y = true; fy = true; }
	// }

	if (resize_x) {
		if (fx) w->pos.width = e.mouse_pos.x - w->pos.x;
		else { w->pos.width -= e.mouse_pos.x - w->pos.x; w->pos.x = e.mouse_pos.x; }
	}

	if (resize_y) {
		if (fy) w->pos.height = e.mouse_pos.y - w->pos.y;
		else { w->pos.height -= e.mouse_pos.y - w->pos.y; w->pos.y = e.mouse_pos.y; }
	}
	printf("resize_x: %d, resize_y: %d %d %d\n", resize_x, resize_y, fx, fy);
	printf("resize_x: %f, resize_y: %f\n", w->pos.x, w->pos.y);
	printf("resize_x: %f, resize_y: %f\n", e.mouse_pos.x, e.mouse_pos.y);
	return (bool)(resize_x | resize_y);
}

int __handle_resize(BIND_FN_PARAMS)
{
	
}

char* __create_widget_name(WIDGET_TYPE type)
{
	const char* type_name = widget_type_as_str(type);
	int offset = 2; // strlen("w.") + 1
	if (__widgets.index >= 10) {
		offset += 2;
	}
	else if (__widgets.index >= 100) {
		offset += 3;
	}
	else if (__widgets.index >= 1000) {
		offset += 4;
	}
	else if (__widgets.index >= 10000) {
		offset += 5;
	}

	char* ret = calloc(offset+strlen(type_name), sizeof(char));
	sprintf(ret, "w.%s%d", type_name, __widgets.index);
	// ret[0] = 'w';
	// ret[1] = '.';
	// strcpy(ret+2, type_name);
	return ret;
}

WIDGET* create_widget(int x, int y, WIDGET_TYPE type, int width, int height, const char* text, STYLE* style)
{
	// WIDGET* w = calloc(1, sizeof(WIDGET));
	if (text == NULL) text = "";
	WIDGET ww;
	WIDGET* w = &ww;
	w->__parent = NULL;
	w->w_name = __create_widget_name(type);
	w->type = type;
	w->disabled = false;

	w->__draw_children = false;
	
	// w->binding_map = calloc(1, sizeof(BINDING_MAP));
	BINDING_MAP_INIT(&w->binding_map, 20);


	w->pos = (Rectangle){
		.x = x, .y = y,
		.width = width,
		.height = height,
	};

	w->vw = (Rectangle){
		.x = 0,
		.y = 0,
		.width = width,
		.height = height,
	};

	size_t len = strlen(text);
	if (len <= 1) len = 2;

	// STRING_INITA(&w->text, &w->str_pool, len);
	STRING_INIT(&w->text, len);
	strcpy(w->text.str, text);
	// STRING_SET(&w->text, w->content_text, len);

	w->cursor.y = 1;
	w->cursor.x = 0;

	if (style == NULL) style = &DEFAULT_STYLE;
	w->style = style;

	w->img = &EMPTY_SPRITE;

	__system_bind_widget(w, "<RESIZE>", __handle_resize);
	__system_bind_widget(w, "[MOUSE_BUTTON_LEFT]", __focus_set);
	__system_bind_widget(w, "<MOUSE_BUTTON_LEFT><MOUSE_MOVE>", test_drag);
	__system_bind_widget(w, "<MOUSE_BUTTON_RIGHT><MOUSE_MOVE>", test_drag_x);
	__system_bind_widget(w, "<KEY_LEFT_SHIFT><MOUSE_BUTTON_RIGHT><MOUSE_MOVE>", test_drag_y);
	__system_bind_widget(w, "[KEY_A]", change_style);
	__system_bind_widget(w, "<MOUSE_WHEEL_MOVE>", test_resize);

	w = WIDGET_VECTOR_ADD(&__widgets, ww);
	return w;
}

void add_child_widget(WIDGET* parent, WIDGET* w)
{
	w->__parent = parent;
	WIDGET_PTR_VECTOR_ADD(&parent->__children, w);
}


void widget_add_img(WIDGET* w, SPRITE* s)
{
	w->img = s;
}


WIDGET* create_frame(int width, int height, STYLE* style)
{
	if (style == NULL) style = &DEFAULT_FRAME_STYLE;
	WIDGET* frame = create_widget(0, 0, W_FRAME, width, height, "", style);
	WIDGET_PTR_VECTOR_INIT(&frame->__children, 10);
	frame->__draw_children = true;
	__system_bind_widget(frame, "<MOUSE_WHEEL_MOVE>", scroll_y);
	bind_widget(frame, "<KEY_LEFT_SHIFT><MOUSE_WHEEL_MOVE>", scroll_x);
	bind_widget(frame, "<MOUSE_BUTTON_LEFT><MOUSE_MOVE>", resize_edge);
	return frame;
}


WIDGET* create_label(const char* text, STYLE* style)
{
	if (style == NULL) style = &DEFAULT_STYLE;
	Vector2 size = MeasureTextEx(style->font, text, style->font_size, style->font_spacing);
	int width = size.x;
	int height = size.y;
	WIDGET* label = create_widget(0, 0, W_LABEL, width, height, text, style);
	return label;
}


int _set_show_children_true(BIND_FN_PARAMS)
{
	focus_set(w);
	w->__draw_children = true;
	// printf("occured %d\n", w->__draw_children);

	return 0;
}

int _set_show_children_false(BIND_FN_PARAMS)
{
	// focus_set(w);
	w->__draw_children = false;
	printf("occured %d\n", w->__draw_children);

	return 0;
}

WIDGET* create_dropdown(const char* text, STYLE* style)
{
	if (style == NULL) style = &DEFAULT_STYLE;
	Vector2 size = MeasureTextEx(style->font, text, style->font_size, style->font_spacing);
	int width = size.x;
	int height = size.y * 2;
	// printf("dropdown size: %d %d %d\n", width, height, style->font_size);
	WIDGET* dropdown = create_widget(0, 0, W_DROPDOWN, width, height, text, style);
	WIDGET_PTR_VECTOR_INIT(&dropdown->__children, 5);
	dropdown->__draw_children = false;
	// TODO: Proper rebinding
	// __system_bind_widget(dropdown, "<MOUSE_BUTTON_LEFT>", _set_show_children);
	__system_bind_widget(dropdown, "[MOUSE_BUTTON_LEFT]", _set_show_children_true);
	__system_bind_widget(dropdown, "<KEY_A>", change_style);
	__system_bind_widget(dropdown, "<LOST_FOCUS>", _set_show_children_false);
	return dropdown;
}


WIDGET* create_button(const char* text, STYLE* style, int (*func) (BIND_FN_PARAMS))
{
	if (style == NULL) style = &DEFAULT_STYLE;
	Vector2 size = MeasureTextEx(style->font, text, style->font_size, style->font_spacing);
	int width = size.x;
	int height = size.y;

	WIDGET* button = create_widget(0, 0, W_BUTTON, width, height, text, style);
	__system_bind_widget(button, "[MOUSE_BUTTON_LEFT]", func);
	return button;
}

int text_input_handle(BIND_FN_PARAMS)
{
	
	// printf("handle_text_input: %c '%s' %d, %d | Vec2<%f, %f>\n\n", e.char_held, w->text.str, w->text.available, w->text.index, w->cursor.y, w->cursor.x);
	// STRING_ADD(&w->text, (char)e.char_held);
	STRING_INSERT(&w->text, (char)e.char_held, w->cursor.x);
	// if (w->lines.items[(int)w->cursor.y]->str == NULL) {
		// STRING_INIT(w->lines.items[(int)w->cursor.y], 15);
		// printf("ok ok ok\n\n");
	// }
	// STRING_ADD(&w->lines.items[(int)w->cursor.y], (char)e.char_held);
	w->cursor.x++;
	return 1;
}

int move_up(BIND_FN_PARAMS);

int text_input_backspace(BIND_FN_PARAMS)
{
	printf("handling backspace\n");
	// STRING_POP(&w->text);
	if (w->cursor.y == 1.f && w->cursor.x == 0.f) return 1;

	w->cursor.x -= 1;

	if (w->cursor.x < 0) {
		// STRING* p = STRING_VECTOR_GET(&w->lines, w->cursor.y);
		move_up(w, e);
		STRING_POP(&w->text);
		w->cursor.x = w->text.index;
		printf("cursor delete: %f, %f : %d\n", w->cursor.y, w->cursor.x, w->lines.index);
		// STRING_MERGE(&w->text, STRING_VECTOR_GET(&w->lines, w->cursor.y));
		STRING* p = STRING_VECTOR_GET(&w->lines, w->cursor.y);
		STRING_MERGE(&w->text, p);
		printf("text: %s %d\n", w->text.str, w->text.index);
		STRING_VECTOR_REMOVE(&w->lines, w->cursor.y);
		// STRING_MERGE(STRING_VECTOR_GET(&w->lines, w->cursor.y-1), w->text);
	}
	else {
		STRING_REMOVE(&w->text, w->cursor.x);
	}
	// w->cursor.x = w->text.index--;
	// if (w->cursor.y <= 0) w->cursor.y = 1;
	// printf("text A: %s %d\n", w->text.str, w->text.index);
	return 1;
}

int print_lines(BIND_FN_PARAMS);

int text_input_newline(BIND_FN_PARAMS)
{
	printf("cursor mewline: %f, %f : %d\n", w->cursor.y, w->cursor.x, w->lines.index);
	// STRING_ADD(&w->text, '\n');
	// STRING_INSERT(&w->text, '\n', w->cursor.x);
	// w->cursor.x++;
	
	if ((int)w->cursor.x == w->text.index) {
		// STRING* p = &w->lines.items[(int)w->cursor.y-1];
		// STRING* p = STRING_VECTOR_GET(&w->lines, w->cursor.y-1)
		// printf("heeeree:333\n");;
		STRING_INSERT(&w->text, '\n', w->cursor.x);
		STRING_VECTOR_REPLACE(&w->lines, w->text, w->cursor.y-1);
		// *p = w->text;
		// STRING_VECTOR_INSERT(*w->lines, w->cursor.y);
		w->cursor.y++;
		w->cursor.x = 0;
		// w->text = *w->lines.items[(int)w->cursor.y];

		// STRING* p = STRING_VECTOR_GET(&w->lines, w->cursor.y-1);
		// STRING p;
		// if (p.str == NULL)
		STRING_INIT(&w->text, 10);
		// STRING_INIT(p, 10);
		// p = STRING_VECTOR_INSERT(&w->lines, *p, w->cursor.y-1);
		// STRING_ADD(&w->text, '\n');
		// w->text = p;
	}

	else {
		STRING p;
		STRING_INIT(&p, (w->text.index-(int)w->cursor.x)*2);
		printf("xx: %d, %d\n", p.index, w->text.index-(int)w->cursor.x);
		STRING_MERGE_F(&p, &w->text, w->cursor.x);


		// STRING_ADD(&w->text, '\0');
		memset(w->text.str + ((int)w->cursor.x), '\0', w->text.available-(int)w->text.index);
		// w->text.str[(int)w->cursor.x] = '\0';
		// STRING_INSERT(&w->text, '\n', w->cursor.x);
		w->text.index = w->cursor.x;
		printf("text: %s %d\n", w->text.str, w->text.index);
		STRING_ADD(&w->text, '\n');
		STRING_VECTOR_INSERT(&w->lines, w->text, w->cursor.y-1);
		w->cursor.y++;

		// STRING_VECTOR_INSERT(&w->lines, p, w->cursor.y);

		w->cursor.x = 0;
		w->text = p;
		printf("text A: %s %d\n", w->text.str, w->text.index);
	}

	printf("cursor mewline A: %f, %f : %d\n", w->cursor.y, w->cursor.x, w->lines.index);
	return 1;
}


int move_up(BIND_FN_PARAMS)
{
	// STRING_ADD(&w->text, '\n');
	printf("cursor up: %f, %f : %d\n", w->cursor.y, w->cursor.x, w->lines.index);
	
	// STRING_VECTOR_REPLACE(&w->lines, w->text, w->cursor.y-1);
	if (w->cursor.y <= 1.f) {
		w->cursor.y = 1.f;
	}

	else {
		printf("cursor up A: %f, %f : %d\n", w->cursor.y, w->cursor.x, w->lines.index);
		STRING_VECTOR_REPLACE(&w->lines, w->text, w->cursor.y-1);
		STRING* p = STRING_VECTOR_GET(&w->lines, w->cursor.y-2);
		if (p->str == NULL) STRING_INIT(p, 10);
		// else w->cursor.x = p->index-2;

		if (w->cursor.x > p->index) w->cursor.x = p->index-1;
		w->cursor.y--;
	
		w->text = *p;
		// w->text.index -= 1;
	}
	return 1;
}

int move_down(BIND_FN_PARAMS)
{
	printf("cursor down: %f, %f : %d\n", w->cursor.y, w->cursor.x, w->lines.index);

	if (w->cursor.y >= w->lines.index) {
		// w->cursor.y--;
		// w->cursor.y = w->lines.index;
	}

	else {
		w->cursor.y++;
		printf("cursor down A: %f, %f : %d\n", w->cursor.y, w->cursor.x, w->lines.index);
	
		// STRING_ADD(&w->text, '\n');
		STRING_VECTOR_REPLACE(&w->lines, w->text, w->cursor.y-2);
		STRING* p = STRING_VECTOR_GET(&w->lines, w->cursor.y-1);
		if (p->str == NULL) { STRING_INIT(p, 10); w->cursor.x = 0; }
		// else w->cursor.x = p->index;

		if (w->cursor.x > p->index) w->cursor.x = p->index-1;
		
		w->text = *p;
	}
	return 1;
}

int move_left(BIND_FN_PARAMS)
{
	printf("cursor left: %f, %f : %d\n", w->cursor.y, w->cursor.x, w->lines.index);
	w->cursor.x--;
	if (w->cursor.x < 0) {
		if (w->cursor.y > 1) { move_up(w, e); w->cursor.x = w->text.index-1; }
		else w->cursor.x++;
	}
	printf("cursor left A: %f, %f : %d\n", w->cursor.y, w->cursor.x, w->lines.index);
	return 1;
}

int move_right(BIND_FN_PARAMS)
{
	printf("cursor right: %f, %f : %d\n", w->cursor.y, w->cursor.x, w->lines.index);
	if (w->cursor.x >= w->text.index || w->text.str[(int)w->cursor.x] == '\n' || w->text.str[(int)w->cursor.x] == '\r' || w->text.str[(int)w->cursor.x+1] == '\n' || w->text.str[(int)w->cursor.x+1] == '\r') {
		if (w->cursor.y < w->lines.index) { move_down(w, e); w->cursor.x = 0; }
	}
	else {
		w->cursor.x++;
	}

	printf("cursor right A: %f, %f : %d\n", w->cursor.y, w->cursor.x, w->lines.index);
	return 1;
}

int move_start_of_line(BIND_FN_PARAMS)
{

	w->cursor.x = 0;
	return 1;
}

int move_end_of_line(BIND_FN_PARAMS)
{

	w->cursor.x = w->text.index-1;
	return 1;
}

int print_lines(BIND_FN_PARAMS)
{
	printf("printing lines\n");
	ITERATE_VECTOR(w->lines, STRING, val) {
		printf("line: %s\n", val->str);
	}
	return 1;
}

WIDGET* create_text_input(const char* text, STYLE* style)
{
	if (style == NULL) style = &DEFAULT_STYLE;
	Vector2 size = MeasureTextEx(style->font, text, style->font_size, style->font_spacing);
	// int width = size.x * 3;
	int width = 1000;
	int height = style->font_size*2;
	if (width < 10) width = height*6;
	// if (height < style->font_size) height = style->font_size;

	WIDGET* text_input = create_widget(0, 0, W_TEXT_INPUT, width, height, text, style);
	__system_bind_widget(text_input, "<KEYPRESS>", text_input_handle);
	__system_bind_widget(text_input, "<KEY_BACKSPACE>", text_input_backspace);
	__system_bind_widget(text_input, "<KEY_ENTER>", text_input_newline);
	__system_bind_widget(text_input, "<KEY_UP>", move_up);
	__system_bind_widget(text_input, "<KEY_DOWN>", move_down);
	__system_bind_widget(text_input, "<KEY_LEFT>", move_left);
	__system_bind_widget(text_input, "<KEY_RIGHT>", move_right);
	__system_bind_widget(text_input, "<KEY_HOME>", move_start_of_line);
	__system_bind_widget(text_input, "[KEY_END]", move_end_of_line);
	__system_unbind_widget(text_input, "[KEY_A]");
	bind_widget(text_input, "[KEY_TAB]", print_lines);
	STRING_VECTOR_INIT(&text_input->lines, 10);
	return text_input;
}


int do_nothing(BIND_FN_PARAMS)
{
	printf("doing nothing\n");
	return 2;
}

void dropdown_add_option(WIDGET* parent, const char* text)
{
	WIDGET* option = create_button(text, parent->style, do_nothing);
	add_child_widget(parent, option);
}


WIDGET* create_child_widget(WIDGET* parent, WIDGET_TYPE type, int width, int height, const char* text)
{
	WIDGET* w = create_widget(0, 0, type, width, height, text, parent->style);
	w->__parent = parent;
	WIDGET_PTR_VECTOR_ADD(&parent->__children, w);
}



void change_widget_style(WIDGET* w, STYLE* style)
{
	if (style == NULL)
		style = &DEFAULT_STYLE;

	w->style = style;
}

void add_widget_to_render_queue(WIDGET* w)
{
	WIDGET_PTR_VECTOR_ADD(&WIDGET_RENDER_QUEUE, w);
}

void remove_widget_from_render_queue(WIDGET* w)
{
	WIDGET_PTR_VECTOR_DELETE(&WIDGET_RENDER_QUEUE, w);
}


float get_string_width(Font font, const char *text, float fontSize, float spacing)
{

	if ((font.texture.id == 0) || (text == NULL)) return 0; // Security check

	int size = TextLength(text);	// Get size in bytes of text
	int tempByteCounter = 0;		// Used to count longer text line num chars
	int byteCounter = 0;

	float textWidth = 0.0f;
	float tempTextWidth = 0.0f;	 // Used to count longer text line width

	float textHeight = fontSize;
	float scaleFactor = fontSize/(float)font.baseSize;

	int letter = 0;				 // Current character
	int index = 0;				  // Index position in sprite font

	for (int i = 0; i < size;)
	{
		byteCounter++;

		int next = 0;
		letter = GetCodepoint(&text[i], &next);
		index = GetGlyphIndex(font, letter);

		i += next;

		if (letter != '\n')
		{
			if (font.glyphs[index].advanceX != 0) textWidth += font.glyphs[index].advanceX;
			else textWidth += (font.recs[index].width + font.glyphs[index].offsetX);
		}

		if (tempByteCounter < byteCounter) tempByteCounter = byteCounter;
	}

	if (tempTextWidth < textWidth) tempTextWidth = textWidth;


	return tempTextWidth*scaleFactor + (float)((tempByteCounter - 1)*spacing);;
}

float get_string_width_untill_i(Font font, const char *text, float fontSize, float spacing, int size)
{

	if ((font.texture.id == 0) || (text == NULL)) return 0; // Security check

	int _size = TextLength(text);	// Get size in bytes of text
	if (size > _size) size = _size;
	
	int tempByteCounter = 0;		// Used to count longer text line num chars
	int byteCounter = 0;

	float textWidth = 0.0f;
	float tempTextWidth = 0.0f;	 // Used to count longer text line width

	float textHeight = fontSize;
	float scaleFactor = fontSize/(float)font.baseSize;

	int letter = 0;				 // Current character
	int index = 0;				  // Index position in sprite font

	for (int i = 0; i < size;)
	{
		byteCounter++;

		int next = 0;
		letter = GetCodepoint(&text[i], &next);
		index = GetGlyphIndex(font, letter);

		i += next;

		if (letter != '\n')
		{
			if (font.glyphs[index].advanceX != 0) textWidth += font.glyphs[index].advanceX;
			else textWidth += (font.recs[index].width + font.glyphs[index].offsetX);
		}

		if (tempByteCounter < byteCounter) tempByteCounter = byteCounter;
	}

	if (tempTextWidth < textWidth) tempTextWidth = textWidth;


    return tempTextWidth*scaleFactor + (float)((tempByteCounter - 1)*spacing);;
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

	const char* line = w->text.str;
	
	// if (w->type == W_TEXT_INPUT) {
		// line = STRING_VECTOR_GET(&w->lines, w->cursor.y)->str;
	// }

	float cursor_offset = get_string_width_untill_i(w->style->font, line, w->style->font_size, w->style->font_spacing, w->cursor.x);
	Vector2 text_size = MeasureTextEx(w->style->font, line, w->style->font_size, w->style->font_spacing);
	Vector2 text_pos;
	Vector2 _text_pos;
	
	switch (w->style->text_justify)
	{

		case JUSTIFY_CENTER:
			text_pos = (Vector2){w->pos.x+w->pos.width/2 - text_size.x/2 + w->style->padding/2 + w->style->border_size/2, w->pos.y+w->style->padding};
		break;

		case JUSTIFY_RIGHT:
			text_pos = (Vector2){w->pos.x+w->pos.width - text_size.x - w->style->padding, w->pos.y+w->style->padding};
		break;

		default:
			text_pos = (Vector2){w->pos.x+w->style->padding, w->pos.y+w->style->padding};
	}

	if (w->type == W_TEXT_INPUT) {
		_text_pos = text_pos;

		float ht = w->pos.height / (w->style->font_size*2);
		float hht = ht / 2;
		int i = -hht;
		for (i; w->cursor.y+i < 1; i++);
		
		for (i; i < ht; i++) {
			// if (w->pos.y+w->style->font_size*i > w->pos.y + w->pos.height) break;
			if (w->cursor.y-1+i > w->lines.index) break;

			if (i == 0) {
				DrawTextEx(w->style->font, w->text.str, _text_pos, w->style->font_size, w->style->font_spacing, w->style->fg);
				text_pos = _text_pos;
			}

			else {
				STRING* p = STRING_VECTOR_GET(&w->lines, w->cursor.y-1+i);
				if (p == NULL || p->str == NULL) break;
				DrawTextEx(w->style->font, p->str, _text_pos, w->style->font_size, w->style->font_spacing, w->style->fg);
			}

			_text_pos.y += w->style->font_size;
		}
		
		DrawRectangleRec(
			(Rectangle){
				.x=text_pos.x+cursor_offset,
				// .y=text_pos.y+w->cursor.y*w->style->font_size,
				.y=text_pos.y,
				.width=10,
				.height=w->style->font_size
			},
			w->style->border_color
		);
	}

	else {
		DrawTextEx(w->style->font, line, text_pos, w->style->font_size, w->style->font_spacing, w->style->fg);
	}

	// if (&w->style->bg_img != &EMPTY_SPRITE)
	// {
		// w->style->bg_img.dst = w->pos;
		// draw_sprite(&w->style->bg_img);
	// }

	if (w->img != &EMPTY_SPRITE)
	{
		// printf("Drawing image: %s\n", w->w_name);
		w->img->dst = w->pos;
		if (w->pos.width < 0.f || w->pos.height < 0.f) printf("\nWHAT\n");
		w->img->src.x = 0;
		w->img->src.y = 0;
		// w->img->src.width = w->pos.width;
		// w->img->src.width = w->pos.height;
		// w->img->dst.x = w->pos.x;
		// w->img->dst.y = w->pos.y;
		draw_sprite(w->img);
	}
	
	if (w->style->border_style == BORDER_SOLID)
		DrawRectangleLinesEx(w->pos, w->style->border_size, w->style->border_color);

	// DrawLine(w->pos.x+w->pos.width/2, w->pos.y, w->pos.x+w->pos.width/2, w->pos.y+w->pos.height, RED);
}

void __draw_gui_pack_horizontal(WIDGET* parent, int x, int y)
{
	int width = parent->pos.width/parent->__children.index;
	ITERATE_VECTOR(parent->__children, WIDGET*, val)
	{
		WIDGET* w = *val;
		w->pos.x = x;
		w->pos.y = y;
		w->pos.width = width;
		w->pos.height = parent->pos.height;
		// printf("what: %d %s %s\n", parent->pos.height/w->__children.index, w->w_name, parent->w_name);
		draw_widget(w);
		// if (w->__draw_children) {
			// __draw_gui(w, x, y);
		// }
		x += w->pos.width;
	}
}

void __draw_gui_pack_vertical(WIDGET* parent, int x, int y)
{
	int height = parent->pos.height/parent->__children.index;
	ITERATE_VECTOR(parent->__children, WIDGET*, val)
	{
		WIDGET* w = *val;
		w->pos.x = x;
		w->pos.y = y;
		w->pos.width = parent->pos.width;
		w->pos.height = height;
		// printf("what: %d %s %s\n", parent->pos.height/w->__children.index, w->w_name, parent->w_name);
		draw_widget(w);
		// if (w->__draw_children) {
			// __draw_gui(w, x, y);
		// }
		y += w->pos.height;
	}
}


void __draw_gui_grid(WIDGET* parent, int x, int y)
{

	int row_height = 0;
	int width, height;
	float _c = -1, _cc = -1, _r = -1;
	int x_offset = 0, y_offset = 0;
	int o_x = x, o_y = y;

	CONTENT_SIZE content_size = __get_content_size(parent);
	width = content_size.width;
	height = content_size.height;
	_c = content_size.columns;
	_r = content_size.rows;

	_cc = parent->pos.height / height;

	x_offset = (parent->pos.width - (width * _c)) / _c;
	x_offset = 0;
	// y_offset = (parent->pos.height - (height * _r)) / _cc;
	y_offset = 0;

	// printf("offset: %d %d %f %d  height: %f %d\n", x_offset, y_offset, parent->pos.width, width, parent->pos.height, height);
	// y += y_offset;

	bool draw_x = false, draw_y = false;
	if (parent->vw.x <= 0) draw_x = true;
	if (parent->vw.y <= 0) draw_y = true;

	if (parent->style->grid.row_width == GRID_AUTO) {
		parent->vw.width = parent->pos.width;
	}
	else parent->vw.width = parent->style->grid.row_width;

	if (parent->style->grid.row_height == GRID_AUTO) {
		parent->vw.height = parent->pos.height;
	}
	else parent->vw.height = parent->style->grid.row_height;

	bool skip_row = false;

	ITERATE_VECTOR(parent->__children, WIDGET*, val)
	{
		WIDGET* w = *val;

		if (o_x+width/2 > parent->pos.x+parent->vw.width) {
			o_x = parent->pos.x;
			x = parent->pos.x;
			o_y += height + y_offset;
			y += height + y_offset;
		}

		// if (row_height < w->pos.height) row_height = w->pos.height;
		// w->pos.x = x + (w->style->margin / 2) + x_offset / 2;
		// w->pos.y = y + w->style->margin / 2 + y_offset / 2;
		// w->pos.width = width - w->style->margin / 2 - x_offset / 2;
		// w->pos.height = height - w->style->margin;

		w->pos.x = x + x_offset / 2;
		w->pos.y = y + y_offset / 2;
		w->pos.width = width - x_offset / 2;
		w->pos.height = height;

		x += w->pos.width  + x_offset;
		o_x += w->pos.width + x_offset;

		if (skip_row) continue;

		if (!draw_x) {
			printf("not drawing: %d %f %f\n", x, w->pos.x+parent->vw.x);
			if (x > parent->pos.x+parent->vw.x) {
				x = parent->pos.x;
				draw_x = true;
			}
			continue;
		}

		if (!draw_y) {
			if (o_y > parent->pos.y+parent->vw.y) {
				// x = parent->pos.x;
				y = parent->pos.y;
				w->pos.y = y;
				draw_y = true;
				// if (x-width > parent->pos.x) {
					// skip_row = true;
				// }
				
			}
			else 
				continue;
		}

		// printf("Drwaing\n");
		// printf("what: %d %s %s\n", parent->pos.height/w->__children.index, w->w_name, parent->w_name);
		// if (x > w->pos.x+parent->vw.x && x < parent->pos.x+parent->pos.width) {
			// w->pos.x -= parent->vw.x;


		if (x < parent->pos.x+parent->pos.width && y+height < parent->pos.y+parent->pos.height) {
			draw_widget(w);
		}

		else {
			bool can_draw = false;
			if (w->pos.x < parent->pos.x+parent->pos.width && x > parent->pos.x+parent->pos.width) {
				float tmp = (w->pos.x+w->pos.width) - (parent->pos.x+parent->pos.width);
				if (tmp < width/2) {
					w->pos.width -=  tmp;
					can_draw = true;
				}
			}

			if (y+height > parent->pos.y+parent->pos.height) can_draw = false;
			// if (y < parent->pos.y+parent->pos.height && y+height > parent->pos.y+parent->pos.height) {
				// can_draw = false;
				// float tmp = (w->pos.y+w->pos.height) - (parent->pos.y+parent->pos.height);
				// if (tmp < height/2) {
					// w->pos.height -=  tmp;
					// can_draw = true;
				// }
			// }
			
			if (can_draw) draw_widget(w);
		}

	}
}

void draw_gui()
{
	// int x = 100;
	// int y = 400;
	
	ITERATE_VECTOR(WIDGET_RENDER_QUEUE, WIDGET*, val)
	{
		WIDGET* w = *val;
		// w->pos.x = x;
		// w->pos.y = y;
		// printf("what: %d %s\n", i, w->text->str);
		draw_widget(w);
		if (w->__draw_children) {
			// printf("drawing for parent: %s\n", w->w_name);
			if (w->type ==  W_DROPDOWN) __draw_gui_pack_vertical(w, w->pos.x, w->pos.y+w->pos.height);
			else if (w->style->use_grid) __draw_gui_grid(w, w->pos.x, w->pos.y);
			else __draw_gui_pack_horizontal(w, w->pos.x, w->pos.y);
		}
		// x = w->pos.x + w->pos.width;
		// y = w->pos.y + w->pos.height;
	}
}


#endif



