#pragma once
#include <raylib.h>
#include <raymath.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "core.h"
#include "util.c"
#include "arena.h"

#ifndef EVENTLOOP_H_
#define EVENTLOOP_H_

float __DELTA_SINCE_LAST_UPDATE = 0.f;

char __EVENT_KEYS[150] = {};
char __EVENT_KEYS_JUST_PRESSED[150] = {};
char __EVENT_BUTTONS[150] = {};
char* __EVENT_MOUSE;
char __EVENT_ALL[300] = {};

// char __EVENT_KEYS_NONBLOCK

bool __BUTTONS_CURRENTLY_HELD[6] = {0, 0, 0, 0, 0};

Vector2 __LAST_MOUSE_POS = {.x=-1, .y=-1};

Vector2 __DRAG_START_POS = {.x=-1, .y=-1};

bool __MOUSE_MOVING = false;
bool __MOUSE_DRAGGING = false;

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
	float mouse_wheel_move;
	Vector2 last_mouse_pos;

	int char_held;

	Vector2 drag_pos;
	
} EVENT;

#define bullshit 1

int a = 0;


#endif