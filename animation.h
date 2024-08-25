#include <raylib.h>
#include <raymath.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#include "core.h"
#include "util.c"
#include "sprite.h"
#include "arena.h"

#ifndef ANIMATION_H_
#define ANIMATION_H_





typedef struct ANIMATION ANIMATION;
typedef struct TRANSFORM_ANIMATION TRANSFORM_ANIMATION;


struct ANIMATION
{
	SPRITE* atlas;
	float frame_rate;

	size_t curr, curr_row;

	int rows;
	int columns;


	int offset_x;
	int offset_y;

	float __delta_since_last_update;
};


void ANIMATION_INIT(ANIMATION* anim, SPRITE* atlas, int frame_rate, int rows, int columns, int offset_x, int offset_y)
{
	anim->atlas = atlas;
	anim->frame_rate = frame_rate;
	anim->rows = rows;
	anim->columns = columns;
	anim->offset_x = offset_x;
	anim->offset_y = offset_y;
	anim->__delta_since_last_update = 0;
}


ANIMATION ANIMATION_CREATE(SPRITE* atlas, int frame_rate, int rows, int columns, int offset_x, int offset_y)
{
	ANIMATION anim;
	anim.atlas = atlas;
	anim.frame_rate = frame_rate;
	anim.rows = rows;
	anim.columns = columns;
	anim.offset_x = offset_x;
	anim.offset_y = offset_y;
	anim.__delta_since_last_update = 0;

	return anim;
}



void animation_step(ANIMATION* anim)
{
	printf("anim: %f %f\n", anim->__delta_since_last_update, 1000/anim->frame_rate*0.001);
	anim->__delta_since_last_update += GetFrameTime();


	if (anim->__delta_since_last_update > 1000/anim->frame_rate*0.001) {
		anim->__delta_since_last_update = 0;
		anim->curr++;
		if (anim->curr > anim->columns) {
			anim->curr_row++;
			if (anim->rows > 0 && anim->curr_row > anim->rows) anim->curr_row = 0;
			anim->curr = 0;
			anim->atlas->src.y += anim->offset_y;
			anim->atlas->src.x = 0;
		}
	
		anim->atlas->src.x += anim->offset_x;
	}
	
	
	
}


void draw_animation(ANIMATION anim)
{
	draw_sprite(anim.atlas);
}

#endif

