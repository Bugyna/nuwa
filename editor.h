#include <raylib.h>
#include <raymath.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#include "util.c"
#include "sprite.h"
#include "arena.h"

#include "gui.h"

#ifndef EDITOR_H_
#define EDITOR_H_


SPRITE_VECTOR EDITOR_RENDER_QUEUE;

bool EDITOR_INITALIZED = false;


bool selected_sprite = false;
SPRITE* selected_sprite_ptr = NULL;
SPRITE* __selected_sprite_empty = NULL;
WIDGET* editor_frame = NULL;

int select_sprite(BIND_FN_PARAMS)
{

	// *selected_sprite = *w->img;
	if (selected_sprite)
		SPRITE_VECTOR_POP(&EDITOR_RENDER_QUEUE);
	selected_sprite_ptr = SPRITE_VECTOR_ADD(&EDITOR_RENDER_QUEUE, *w->img);
	selected_sprite = true;
	printf("selected sprite: %s\n", selected_sprite_ptr->path);
	
	// bind_widget(
	return 1;
}

int place_sprite(BIND_FN_PARAMS)
{
	// if (__WIDGET_FOCUS != w && __WIDGET_ATTENTION == w) return 0;
	printf("whatt\n");
	if (selected_sprite)
	{
		selected_sprite_ptr = __selected_sprite_empty;
		selected_sprite = false;
	}
	return 1;
}

int update_selected_sprite_pos(BIND_FN_PARAMS)
{
	selected_sprite_ptr->dst.x = e.mouse_pos.x;
	selected_sprite_ptr->dst.y = e.mouse_pos.y;
	return 1;
}



#endif
