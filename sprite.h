#include <raylib.h>
#include <raymath.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#include "util.c"
#include "SLL/linked_list.h"


#ifndef SPRITE_H_
#define SPRITE_H_



typedef struct
{
	Image img;
	Texture2D tex;
	Rectangle src, dst;
} SPRITE;

DEFINE_LINKED_LIST(SPRITE_LIST, SPRITE)

typedef struct
{
	SPRITE_LIST sprites;
	
} LAYER;



SPRITE create_sprite_from_image(const char* path)
{
	SPRITE s;
	s.img = LoadImage(path);
	s.tex = LoadTextureFromImage(s.img);
}


SPRITE create_sprite_from_image_f(const char* path, int w, int h)
{
	SPRITE s;
	s.img = LoadImage(path);
	ImageResize(&s.img, w, h);
	s.tex = LoadTextureFromImage(s.img);
	// UnloadImage(s.img);   // Once image has been converted to texture and uploaded to VRAM, it can be unloaded from RAM
}

void unload_sprite(SPRITE* s)
{
	UnloadImage(s->img);
	UnloadTexture(s->tex);
}

#endif