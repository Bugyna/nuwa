#pragma once
#include <raylib.h>
#include <raymath.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "util.c"
// #include "gui.h"
// #include "arena.h"
// #include "eventloop.h"

// DEFINE_LINKED_LIST(KEYCODE_LIST, int)
// KEYCODE_LIST KEYS_HELD;


// const char* __SYSTEM_KEYS_STRING[] = {"KEY_NULL", "KEY_APOSTROPHE", "KEY_COMMA", "KEY_MINUS", "KEY_PERIOD", "KEY_SLASH", "KEY_ZERO", "KEY_ONE", "KEY_TWO", "KEY_THREE", "KEY_FOUR", "KEY_FIVE", "KEY_SIX", "KEY_SEVEN", "KEY_EIGHT", "KEY_NINE", "KEY_SEMICOLON", "KEY_EQUAL", "KEY_A", "KEY_B", "KEY_C", "KEY_D", "KEY_E", "KEY_F", "KEY_G", "KEY_H", "KEY_I", "KEY_J", "KEY_K", "KEY_L", "KEY_M", "KEY_N", "KEY_O", "KEY_P", "KEY_Q", "KEY_R", "KEY_S", "KEY_T", "KEY_U", "KEY_V", "KEY_W", "KEY_X", "KEY_Y", "KEY_Z", "KEY_LEFT_BRACKET", "KEY_BACKSLASH", "KEY_RIGHT_BRACKET", "KEY_GRAVE", "KEY_SPACE", "KEY_ESCAPE", "KEY_ENTER", "KEY_TAB", "KEY_BACKSPACE", "KEY_INSERT", "KEY_DELETE", "KEY_RIGHT", "KEY_LEFT", "KEY_DOWN", "KEY_UP", "KEY_PAGE_UP", "KEY_PAGE_DOWN", "KEY_HOME", "KEY_END", "KEY_CAPS_LOCK", "KEY_SCROLL_LOCK", "KEY_NUM_LOCK", "KEY_PRINT_SCREEN", "KEY_PAUSE", "KEY_F1", "KEY_F2", "KEY_F3", "KEY_F4", "KEY_F5", "KEY_F6", "KEY_F7", "KEY_F8", "KEY_F9", "KEY_F10", "KEY_F11", "KEY_F12", "KEY_LEFT_SHIFT", "KEY_LEFT_CONTROL", "KEY_LEFT_ALT", "KEY_LEFT_SUPER", "KEY_RIGHT_SHIFT", "KEY_RIGHT_CONTROL", "KEY_RIGHT_ALT", "KEY_RIGHT_SUPER", "KEY_KB_MENU", "KEY_KP_0", "KEY_KP_1", "KEY_KP_2", "KEY_KP_3", "KEY_KP_4", "KEY_KP_5", "KEY_KP_6", "KEY_KP_7", "KEY_KP_8", "KEY_KP_9", "KEY_KP_DECIMAL", "KEY_KP_DIVIDE", "KEY_KP_MULTIPLY", "KEY_KP_SUBTRACT", "KEY_KP_ADD", "KEY_KP_ENTER", "KEY_KP_EQUAL", "KEY_BACK", "KEY_MENU", "KEY_VOLUME_UP", "KEY_VOLUME_DOWN"};


const char* get_key_name(int key)
{
	switch (key) {
		case 0:   return  "KEY_NULL";
		case 39:  return  "KEY_APOSTROPHE";
		case 44:  return  "KEY_COMMA";
		case 45:  return  "KEY_MINUS";
		case 46:  return  "KEY_PERIOD";
		case 47:  return  "KEY_SLASH";
		case 48:  return  "KEY_ZERO";
		case 49:  return  "KEY_ONE";
		case 50:  return  "KEY_TWO";
		case 51:  return  "KEY_THREE";
		case 52:  return  "KEY_FOUR";
		case 53:  return  "KEY_FIVE";
		case 54:  return  "KEY_SIX";
		case 55:  return  "KEY_SEVEN";
		case 56:  return  "KEY_EIGHT";
		case 57:  return  "KEY_NINE";
		case 59:  return  "KEY_SEMICOLON";
		case 61:  return  "KEY_EQUAL";
		case 65:  return  "KEY_A";
		case 66:  return  "KEY_B";
		case 67:  return  "KEY_C";
		case 68:  return  "KEY_D";
		case 69:  return  "KEY_E";
		case 70:  return  "KEY_F";
		case 71:  return  "KEY_G";
		case 72:  return  "KEY_H";
		case 73:  return  "KEY_I";
		case 74:  return  "KEY_J";
		case 75:  return  "KEY_K";
		case 76:  return  "KEY_L";
		case 77:  return  "KEY_M";
		case 78:  return  "KEY_N";
		case 79:  return  "KEY_O";
		case 80:  return  "KEY_P";
		case 81:  return  "KEY_Q";
		case 82:  return  "KEY_R";
		case 83:  return  "KEY_S";
		case 84:  return  "KEY_T";
		case 85:  return  "KEY_U";
		case 86:  return  "KEY_V";
		case 87:  return  "KEY_W";
		case 88:  return  "KEY_X";
		case 89:  return  "KEY_Y";
		case 90:  return  "KEY_Z";
		case 91:  return  "KEY_LEFT_BRACKET";
		case 92:  return  "KEY_BACKSLASH";
		case 93:  return  "KEY_RIGHT_BRACKET";
		case 96:  return  "KEY_GRAVE";
		case 32:  return  "KEY_SPACE";
		case 256: return  "KEY_ESCAPE";
		case 257: return  "KEY_ENTER";
		case 258: return  "KEY_TAB";
		case 259: return  "KEY_BACKSPACE";
		case 260: return  "KEY_INSERT";
		case 261: return  "KEY_DELETE";
		case 262: return  "KEY_RIGHT";
		case 263: return  "KEY_LEFT";
		case 264: return  "KEY_DOWN";
		case 265: return  "KEY_UP";
		case 266: return  "KEY_PAGE_UP";
		case 267: return  "KEY_PAGE_DOWN";
		case 268: return  "KEY_HOME";
		case 269: return  "KEY_END";
		case 280: return  "KEY_CAPS_LOCK";
		case 281: return  "KEY_SCROLL_LOCK";
		case 282: return  "KEY_NUM_LOCK";
		case 283: return  "KEY_PRINT_SCREEN";
		case 284: return  "KEY_PAUSE";
		case 290: return  "KEY_F1";
		case 291: return  "KEY_F2";
		case 292: return  "KEY_F3";
		case 293: return  "KEY_F4";
		case 294: return  "KEY_F5";
		case 295: return  "KEY_F6";
		case 296: return  "KEY_F7";
		case 297: return  "KEY_F8";
		case 298: return  "KEY_F9";
		case 299: return  "KEY_F10";
		case 300: return  "KEY_F11";
		case 301: return  "KEY_F12";
		case 340: return  "KEY_LEFT_SHIFT";
		case 341: return  "KEY_LEFT_CONTROL";
		case 342: return  "KEY_LEFT_ALT";
		case 343: return  "KEY_LEFT_SUPER";
		case 344: return  "KEY_RIGHT_SHIFT";
		case 345: return  "KEY_RIGHT_CONTROL";
		case 346: return  "KEY_RIGHT_ALT";
		case 347: return  "KEY_RIGHT_SUPER";
		case 348: return  "KEY_KB_MENU";
		case 320: return  "KEY_KP_0";
		case 321: return  "KEY_KP_1";
		case 322: return  "KEY_KP_2";
		case 323: return  "KEY_KP_3";
		case 324: return  "KEY_KP_4";
		case 325: return  "KEY_KP_5";
		case 326: return  "KEY_KP_6";
		case 327: return  "KEY_KP_7";
		case 328: return  "KEY_KP_8";
		case 329: return  "KEY_KP_9";
		case 330: return  "KEY_KP_DECIMAL";
		case 331: return  "KEY_KP_DIVIDE";
		case 332: return  "KEY_KP_MULTIPLY";
		case 333: return  "KEY_KP_SUBTRACT";
		case 334: return  "KEY_KP_ADD";
		case 335: return  "KEY_KP_ENTER";
		case 336: return  "KEY_KP_EQUAL";
		case 4:   return  "KEY_BACK";
		case 5:   return  "KEY_MENU";
		case 24:  return  "KEY_VOLUME_UP";
		case 25:  return  "KEY_VOLUME_DOWN";
	}
}




const char* get_mouse_button_name(int button)
{
	switch (button)
	{
		case 0: return "MOUSE_BUTTON_LEFT";
	   case 1: return "MOUSE_BUTTON_RIGHT";
		case 2: return "MOUSE_BUTTON_MIDDLE";
		case 3: return "MOUSE_BUTTON_SIDE";
		case 4: return "MOUSE_BUTTON_EXTRA";
		case 5: return "MOUSE_BUTTON_FORWARD";
		case 6: return "MOUSE_BUTTON_BACK";
	}
}


const char* u32_to_utf8_string(int c)
{
	uint8_t* ret = NULL;
	if (c == 0) return NULL;
	
	if (c > 0xFFFF) {
		// printf("4 bytes\n");
		// ret = calloc(5, sizeof(char)); ret[3] = c & 0b10111111; ret[2] = (c >> 8) & 0b10111111; ret[1] = (c >> 16) & 0b10111111; ret[0] = (c >> 24) & 0b11110111;
		ret = calloc(3, sizeof(uint8_t)); 
		ret[0] = (c >> 19) | 0b11110000;
		ret[1] = (c >> 13) & 0b10111111;
		ret[2] = (c >> 7) & 0b10111111;
		ret[3] = ((uint8_t)c | 0b11000000) & 0b10111111;
	}
	
	else if (c > 0x07FF) {
		ret = calloc(3, sizeof(uint8_t)); 
		ret[0] = (c >> 12) | 0b11100000;
		ret[1] = (c >> 6) & 0b10111111;
		ret[2] = ((uint8_t)c | 0b11000000) & 0b10111111;
		// printf("3 bytes: 0x%x 0x%x 0x%x\n", ret[0], ret[1], ret[2]);
	}
	
	
	else if (c > 0x007F) {
		// printf("2 bytes\n");
		ret = calloc(2, sizeof(uint8_t));
		ret[0] = (c >> 6) | 0b11000000;
		ret[1] = ((uint8_t)c | 0b11000000) & 0b10111111;
		// printf("2 bytes: 0x%x 0x%x\n", ret[0], ret[1]);
	}

	else {
		// printf("1 bytes\n");
		ret = calloc(1, sizeof(char)); ret[0] = c & 0b01111111;
	}
	
	return ret;

}


void event_process_held_keys()
{
	const char* tmp = NULL;
	size_t ki = 0;
	for (int i = 0; i < __KEYS_HELD_INDEX; i++)
	// for (int i = 0; i < 360; i++)
	{
		if (IsKeyDown(__KEYS_HELD[i])) {
			tmp = get_key_name(__KEYS_HELD[i]);
			strcat(__EVENT_KEYS, "<");
			strcat(__EVENT_KEYS, tmp);
			strcat(__EVENT_KEYS, ">");
			// ki+=strlen(tmp);
			// printf("ki: %d\n", ki);
		}

		// if (IsKeyPressed(__KEYS_HELD[i])) {
			// printf("aaaaa: %s\n", get_key_name(__KEYS_HELD[i]));
		// }
		else {
			tmp = get_key_name(__KEYS_HELD[i]);
			strcat(__EVENT_KEYS, "(");
			strcat(__EVENT_KEYS, tmp);
			strcat(__EVENT_KEYS, ")");
			memmove(&__KEYS_HELD[i], &__KEYS_HELD[i+1], (__KEYS_HELD_INDEX-i)*sizeof(int));
			__KEYS_HELD_INDEX--;
			i--;
		}
	}
}

const char* event_handle_keyboard()
{
	
	__CHARS_BUFFER_INDEX = 0;
	memset(__EVENT_KEYS , 0, 150);
	memset(__EVENT_KEYS_JUST_PRESSED , 0, 150);
	memset(__CHARS_BUFFER, 0, 150);
	const char* tmp = NULL;


	bool should_process_held_keys = false;
	// event_process_held_keys();
	
	int key = GetCharPressed();	
	while (key != 0) {
		tmp = u32_to_utf8_string(key);
		__CHARS_BUFFER[__CHARS_BUFFER_INDEX++] = key;
		printf("charpresssed: %d\n", key);
		// strcat(__CHARS_BUFFER, tmp);
		// printf("key: %s\n", tmp);
		// printf("key: %i %s \n", key, get_key_name(key1));
		// printf("key: %i\n", key);
		key = GetCharPressed();
		should_process_held_keys = true;
	}


	int key1 = GetKeyPressed();
	while (key1 != 0) {
		__KEYS_HELD[__KEYS_HELD_INDEX++] = key1;
		tmp = get_key_name(key1);
		strcat(__EVENT_KEYS_JUST_PRESSED, "[");
		strcat(__EVENT_KEYS_JUST_PRESSED, tmp);
		strcat(__EVENT_KEYS_JUST_PRESSED, "]");
		// tmp = get_key_name(key1);
		// strcat(__EVENT_KEYS, "<");
		// strcat(__EVENT_KEYS, tmp);
		// strcat(__EVENT_KEYS, ">");
		should_process_held_keys = true;
		__DELTA_SINCE_LAST_UPDATE = -0.1;
		printf("key pressed: %s\n", get_key_name(key1));
		key1 = GetKeyPressed();
	}


	// event_process_held_keys();
	__DELTA_SINCE_LAST_UPDATE += GetFrameTime();
	if (should_process_held_keys) {
		event_process_held_keys();
	}
	
	else {
		if (__DELTA_SINCE_LAST_UPDATE >= 0.033 && __KEYS_HELD_INDEX > 0) {
			event_process_held_keys();
			__DELTA_SINCE_LAST_UPDATE = 0;
		}
	}


	// else {
		// tmp = NULL;
		// size_t ki = 0;
		// for (int i = 0; i < __KEYS_HELD_INDEX; i++)
		// {
			// if (!IsKeyDown(__KEYS_HELD[i])) {
				// tmp = get_key_name(__KEYS_HELD[i]);
				// strcat(__EVENT_KEYS, "(");
				// strcat(__EVENT_KEYS, tmp);
				// strcat(__EVENT_KEYS, ")");
				// memmove(&__KEYS_HELD[i], &__KEYS_HELD[i+1], (__KEYS_HELD_INDEX-i)*sizeof(int));
				// __KEYS_HELD_INDEX--;
				// i--;
			// }
		// }
	// }

	// printf("delta: %f %f %f\n", GetFrameTime(), GetTime(), __DELTA_SINCE_LAST_UPDATE);


	// char keys[100];

	// for (int i = 0; i < 360; i++)
	// {
		// if (IsKeyDown(i)) {
			// printf("key: %s\n", get_key_name(i));
		// }
	// }
	// if (tmp != NULL)
		// printf("Keys: %s\n", __EVENT_KEYS);

	return __EVENT_KEYS;
}


const char* event_handle_mouse()
{
	// char __EVENT_BUTTONS[100] = {'\0'};
	memset(__EVENT_BUTTONS, 0, 150);
	for (int i = 0; i < 6; i++) {
		if (IsMouseButtonDown(i)) {
			if (!__BUTTONS_CURRENTLY_HELD[i]) {
				strcat(__EVENT_BUTTONS, "[");
				strcat(__EVENT_BUTTONS, get_mouse_button_name(i));
				strcat(__EVENT_BUTTONS, "]");
			}

			else {
				strcat(__EVENT_BUTTONS, "<");
				strcat(__EVENT_BUTTONS, get_mouse_button_name(i));
				strcat(__EVENT_BUTTONS, ">");
			}
		}
		__BUTTONS_CURRENTLY_HELD[i] = IsMouseButtonDown(i);
	}

	// float mouse_wheel = GetMouseWheelMove();
	// if (mouse_wheel) {
		// strcat(__EVENT_BUTTONS, "<MOUSE_WHEEL_MOVE>");
		
	// }
	// if (__EVENT_BUTTONS[0] != '\0')
		// printf("buttons: %s\n", __EVENT_BUTTONS);

	// Vector2 mouse_wheel = GetMouseWheelMoveV();
	// if (mouse_wheel.x || mouse_wheel.y) printf("wheel: %f.%f\n", mouse_wheel.x, mouse_wheel.y);
	return __EVENT_BUTTONS;
}



void event_handle()
{
	const char* keys = event_handle_keyboard();
	const char* buttons = event_handle_mouse();
	bool mouse_move = false;
	
	// Vector2 mouse_delta = GetMouseDelta();
	float mouse_wheel = GetMouseWheelMove();
	Vector2 mouse_position = GetMousePosition();
	
	EVENT e = (EVENT){
		.mouse_pos = mouse_position,
		.mouse_delta = (Vector2){
			.x = mouse_position.x - __LAST_MOUSE_POSITION.x,
			.y = mouse_position.y - __LAST_MOUSE_POSITION.y
		},
		
		.mouse_pos_rel = (Vector2){
			.x = abs(__WIDGET_FOCUS->pos.x - mouse_position.x),
			.y = abs(__WIDGET_FOCUS->pos.y - mouse_position.y)
		},

		.mouse_delta_rel = (Vector2){
			.x = abs(__WIDGET_FOCUS->pos.x - __LAST_MOUSE_POSITION.x),
			.y = abs(__WIDGET_FOCUS->pos.y - __LAST_MOUSE_POSITION.y)
		},

		.mouse_wheel_move = mouse_wheel,

		.char_held = __CHARS_BUFFER[__CHARS_BUFFER_INDEX-1],
		
	};
	
	memset(__EVENT_ALL, 0, 300);
	strcpy(__EVENT_ALL, __EVENT_KEYS);
	
	if (mouse_wheel) {
		strcat(__EVENT_ALL+strlen(__EVENT_ALL), "<MOUSE_WHEEL_MOVE>");
		// TODO: mouse_wheel event
		// printf("wheel: %f\n", mouse_wheel);
		
	}


	WIDGET* last_attention = __WIDGET_ATTENTION;
	__EVENT_MOUSE = __EVENT_ALL+strlen(__EVENT_ALL);
	strcpy(__EVENT_MOUSE, __EVENT_BUTTONS);
	// if (mouse_delta.x || mouse_delta.y) {
	if (mouse_position.x != __LAST_MOUSE_POSITION.x || mouse_position.y != __LAST_MOUSE_POSITION.y) {
		// printf("<MOUSE_MOVE> %f.%f\n", mouse_position.x, mouse_position.y);
		mouse_move = true;
		__LAST_MOUSE_POSITION = mouse_position;
		strcpy(__EVENT_ALL+strlen(__EVENT_ALL), "<MOUSE_MOVE>");
	}


	__WIDGET_ATTENTION = WINDOW_WIDGET;

	ITERATE_VECTOR(__widgets, WIDGET, val)
	{	
		// printf("check: %s\n", val->w_name);
		if (CheckCollisionPointRec(mouse_position, val->pos)) {
			__WIDGET_ATTENTION = val;
		}
	}
	// if (__WIDGET_ATTENTION != NULL) {
		// printf("WW: %s\n", __WIDGET_ATTENTION->w_name);
		// __WIDGET_FOCUS = __WIDGET_ATTENTION;
	// }
	// if (__WIDGET_ATTENTION != __WIDGET_FOCUS)
	// if (__WIDGET_ATTENTION != &__WINDOW_WIDGET) {
		// execute_widget_bind(__WIDGET_ATTENTION, __EVENT_ALL, e);
	// }

	bool executed_keys = 0;
	WIDGET* last_focus = __WIDGET_FOCUS;

	if (IsWindowResized()) 
	{
		execute_widget_bind(WINDOW_WIDGET, "<WINDOW_RESIZED>", e);
	}

	// execute_widget_bind(__WIDGET_FOCUS, __EVENT_ALL, e);
	
	if (__WIDGET_FOCUS != __WIDGET_ATTENTION && (!mouse_move || __WIDGET_FOCUS != __WIDGET_LOCK)) {
		int ret = 0;
		ret = execute_widget_bind(__WIDGET_ATTENTION, __EVENT_BUTTONS, e);
		ret = execute_widget_bind(__WIDGET_ATTENTION, __EVENT_MOUSE, e);
		
		
		execute_widget_bind(__WIDGET_FOCUS, __EVENT_ALL, e);
		execute_widget_bind(__WIDGET_FOCUS, __EVENT_KEYS_JUST_PRESSED, e);
		if (strcmp(__EVENT_KEYS, __EVENT_ALL) != 0) execute_widget_bind(__WIDGET_FOCUS, __EVENT_KEYS, e);
		// ret |= execute_widget_bind(__WIDGET_ATTENTION, __EVENT_ALL, e);
		// if (ret) __WIDGET_LOCK = __WIDGET_FOCUS;
	}
	
	else if (__WIDGET_FOCUS == __WIDGET_ATTENTION || (mouse_move && __WIDGET_FOCUS == __WIDGET_LOCK)) {
		int ret = 0;
		// if (!strcmp(__EVENT_BUTTONS, __EVENT_ALL)) ret = execute_widget_bind(__WIDGET_ATTENTION, __EVENT_BUTTONS, e);
		// ret = execute_widget_bind(__WIDGET_FOCUS, __EVENT_MOUSE, e);
		ret = execute_widget_bind(__WIDGET_FOCUS, __EVENT_ALL, e);
		execute_widget_bind(__WIDGET_FOCUS, __EVENT_KEYS_JUST_PRESSED, e);
		if (strcmp(__EVENT_KEYS, __EVENT_ALL) != 0) execute_widget_bind(__WIDGET_FOCUS, __EVENT_KEYS, e);
		__WIDGET_LOCK = EMPTY_WIDGET;
		if (mouse_move && ret) __WIDGET_LOCK = __WIDGET_FOCUS;
	}

	if (__CHARS_BUFFER[0] != '\0') {
		execute_widget_bind(__WIDGET_FOCUS, "<KEYPRESS>", e);
	}

	if (__WIDGET_FOCUS != last_focus) {
		printf("lost_focus\n");
		execute_widget_bind(last_focus, "<LOST_FOCUS>", e);
		execute_widget_bind(__WIDGET_FOCUS, "<FOCUS>", e);
	}


	if (__WIDGET_ATTENTION != last_attention) {
		execute_widget_bind(last_attention, "<LOST_ATTENTION>", e);
		execute_widget_bind(__WIDGET_ATTENTION, "<ATTENTION>", e);
	}
	// if (last_focus != __WIDGET_FOCUS) execute_widget_bind(__WIDGET_FOCUS, __EVENT_KEYS, e);
	// if (strcmp(__EVENT_KEYS, __EVENT_ALL) == 0 && last_focus != __WIDGET_FOCUS) execute_widget_bind(__WIDGET_FOCUS, __EVENT_KEYS, e);

	
	
	if (__EVENT_ALL[0]) {
		// printf("all: %s %s %s %d %f, %f\n%s\n", __EVENT_ALL, __EVENT_KEYS, __EVENT_KEYS_JUST_PRESSED, strcmp(__EVENT_KEYS, __EVENT_ALL), mouse_position.x, mouse_position.y, __CHARS_BUFFER);
	// printf("all: %s\n", __CHARS_BUFFER);
		// printf("FLUSH\n\n\n");
	}

	// else printf("\n\n");


}

