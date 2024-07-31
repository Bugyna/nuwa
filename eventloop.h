#pragma once
#include <raylib.h>
#include <raymath.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "util.c"
#include "arena.h"

#ifndef EVENTLOOP_H_
#define EVENTLOOP_H_
char __EVENT_KEYS[150];
char __EVENT_KEYS_JUST_PRESSED[150];
char __EVENT_BUTTONS[150];
char* __EVENT_MOUSE;
char __EVENT_ALL[300];

Vector2 __LAST_MOUSE_POSITION = {.x=-1, .y=-1};




static int __CHARS_BUFFER[150];
int __CHARS_BUFFER_INDEX = 0;
static int __KEYS_HELD[360];
int __KEYS_HELD_INDEX = 0;
const char* get_key_name(int key);
const char* get_mouse_button_name(int button);
void event_handle();

// DEFINE_HASHMAP(__SYSTEM_KEYBINDS_MAP, int)
// __SYSTEM_KEYBINDS_MAP __system_keybinds;



// void init_eventloop()
// {
	// __SYSTEM_KEYBINDS_INIT(&__system_keybinds, 109);
	
// }

typedef struct
{
	Vector2 mouse_pos;
	Vector2 mouse_delta;
	Vector2 mouse_pos_rel;
	Vector2 mouse_delta_rel;

	
} EVENT;


#endif