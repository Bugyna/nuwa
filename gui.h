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
typedef struct WIDGET WIDGET;

typedef struct BINDING BINDING;



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
STYLE DEFAULT_FRAME_STYLE;
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
	.text = (STRING){.str="", .index=0, .available=0},
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
WIDGET_PTR_VECTOR WIDGET_RENDER_QUEUE;
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
	if (b->system != NULL) ret = b->system(w, e);
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
		.width = width, .height = height
	};

	size_t len = strlen(text);
	if (len <= 1) len = 2;

	STRING_INIT(&w->text, len);
	strcpy(w->text.str, text);
	// STRING_SET(&w->text, w->content_text, len);

	w->cursor.y = 1;
	w->cursor.x = 0;

	if (style == NULL) style = &DEFAULT_STYLE;
	w->style = style;

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


WIDGET* create_frame(int width, int height, STYLE* style)
{
	if (style == NULL) style = &DEFAULT_FRAME_STYLE;
	WIDGET* frame = create_widget(0, 0, W_FRAME, width, height, "", style);
	WIDGET_PTR_VECTOR_INIT(&frame->__children, 10);
	frame->__draw_children = true;
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
	
	printf("handle_text_input: %c '%s' %d, %d | Vec2<%f, %f>\n\n", e.char_held, w->text.str, w->text.available, w->text.index, w->cursor.y, w->cursor.x);
	// STRING_ADD(&w->text, (char)e.char_held);
	STRING_INSERT(&w->text, (char)e.char_held, w->cursor.x);
	// if (w->lines.items[(int)w->cursor.y]->str == NULL) {
		// STRING_INIT(w->lines.items[(int)w->cursor.y], 15);
		// printf("ok ok ok\n\n");
	// }
	// STRING_ADD(&w->lines.items[(int)w->cursor.y], (char)e.char_held);
	w->cursor.x++;
}

int text_input_backspace(BIND_FN_PARAMS)
{
	printf("handling backspace\n");
	// STRING_POP(&w->text);
	w->cursor.x -= 1;
	STRING_REMOVE(&w->text, w->cursor.x);
	if (w->cursor.x < 0) w->cursor.y -= 1;
	if (w->cursor.y <= 0) w->cursor.y = 1;
}

int text_input_newline(BIND_FN_PARAMS)
{
	
	// STRING_ADD(&w->text, '\n');
	// STRING* p = &w->lines.items[(int)w->cursor.y-1];
	STRING* p = STRING_VECTOR_GET(&w->lines, w->cursor.y-1);
	STRING_VECTOR_REPLACE(&w->lines, w->text, w->cursor.y-1);
	// *p = w->text;
	// STRING_VECTOR_INSERT(*w->lines, w->cursor.y);
	w->cursor.y++;
	w->cursor.x = 0;
	// w->text = *w->lines.items[(int)w->cursor.y];
	
	p = STRING_VECTOR_GET(&w->lines, w->cursor.y-1);
	if (p->str == NULL) STRING_INIT(p, 10);
	// STRING_INIT(p, 10);
	// p = STRING_VECTOR_INSERT(&w->lines, *p, w->cursor.y-1);
	w->text = *p;
}


int move_up(BIND_FN_PARAMS)
{
	// STRING_ADD(&w->text, '\n');
	printf("cursor up: %f, %f : %d\n", w->cursor.y, w->cursor.x, w->lines.index);
	
	// STRING_VECTOR_REPLACE(&w->lines, w->text, w->cursor.y-1);
	w->cursor.y--;
	if (w->cursor.y <= 0.f) {
		w->cursor.y = 1.f;
	}

	else {
		printf("cursor up A: %f, %f : %d\n", w->cursor.y, w->cursor.x, w->lines.index);
		STRING_VECTOR_REPLACE(&w->lines, w->text, w->cursor.y);
		STRING* p = STRING_VECTOR_GET(&w->lines, w->cursor.y-1);
		if (p->str == NULL) STRING_INIT(p, 10);
		// else w->cursor.x = p->index-2;
	
		w->text = *p;
		// w->text.index -= 1;
	}
}

int move_down(BIND_FN_PARAMS)
{
	printf("cursor down: %f, %f : %d\n", w->cursor.y, w->cursor.x, w->lines.index);

	w->cursor.y++;
	if (w->cursor.y-1 >= w->lines.index) {
		// w->cursor.y--;
		w->cursor.y = w->lines.index;
	}

	else {
		printf("cursor down A: %f, %f : %d\n", w->cursor.y, w->cursor.x, w->lines.index);
	
		// STRING_ADD(&w->text, '\n');
		STRING_VECTOR_REPLACE(&w->lines, w->text, w->cursor.y-2);
		STRING* p = STRING_VECTOR_GET(&w->lines, w->cursor.y-1);
		if (p->str == NULL) { STRING_INIT(p, 10); w->cursor.x = 0; }
		// else w->cursor.x = p->index;
		
		w->text = *p;
	}
}

int move_left(BIND_FN_PARAMS)
{
	w->cursor.x--;
	if (w->cursor.x < 0) move_up(w, e);
}

int move_right(BIND_FN_PARAMS)
{
	w->cursor.x++;
	if (w->cursor.x > w->text.index) move_down(w, e);
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
	__system_bind_widget(text_input, "[KEY_ENTER]", text_input_newline);
	__system_bind_widget(text_input, "[KEY_UP]", move_up);
	__system_bind_widget(text_input, "[KEY_DOWN]", move_down);
	__system_bind_widget(text_input, "<KEY_LEFT>", move_left);
	__system_bind_widget(text_input, "<KEY_RIGHT>", move_right);
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

	DrawTextEx(w->style->font, line, text_pos, w->style->font_size, w->style->font_spacing, w->style->fg);

	if (w->type == W_TEXT_INPUT) {
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

void draw_gui()
{
	// int x = 100;
	// int y = 400;
	
	ITERATE_VECTOR(WIDGET_RENDER_QUEUE, WIDGET*, val)
	{
		WIDGET* w = *val;
		// w->pos.x = x;
		// w->pos.y = y;
		// printf("what: %d %s\n", i, w->text.str);
		draw_widget(w);
		if (w->__draw_children) {
			// printf("drawing for parent: %s\n", w->w_name);
			if (w->type ==  W_DROPDOWN) __draw_gui_pack_vertical(w, w->pos.x, w->pos.y+w->pos.height);
			else __draw_gui_pack_horizontal(w, w->pos.x, w->pos.y);
		}
		// x = w->pos.x + w->pos.width;
		// y = w->pos.y + w->pos.height;
	}
}


#endif



