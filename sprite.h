#include <raylib.h>
#include <raymath.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#include "util.c"
#include "arena.h"
#include "SLL/linked_list.h"


#ifndef SPRITE_H_
#define SPRITE_H_



typedef struct
{
	char* path;
	Texture2D tex;
	Rectangle src, dst;
	Vector2 cosi;
} SPRITE;

DEFINE_VECTOR(SPRITE_VECTOR, SPRITE)
DEFINE_VECTOR(SPRITE_PTR_VECTOR, SPRITE*)
DEFINE_LINKED_LIST(SPRITE_LIST, SPRITE)

typedef struct LAYER
{
	SPRITE_VECTOR sprites;
	int index;

} LAYER;


void draw_sprite(SPRITE* s);


void layer_init(LAYER* l, int index)
{
	SPRITE_VECTOR_INIT(&l->sprites, 30);
}


void draw_layer(LAYER* l)
{
	ITERATE_VECTOR(l->sprites, SPRITE, sprite)
	{
		draw_sprite(sprite);
	}
}



SPRITE create_sprite_from_image(char* path)
{
	SPRITE s;
	s.path = malloc(strlen(path));
	strcpy(s.path, path);
	Image img = LoadImage(path);
	s.tex = LoadTextureFromImage(img);

	s.cosi = (Vector2){.x=0, .y=0};

	s.dst = (Rectangle){
		.x = 0.f,
		.y = 0.f,
		.width = s.tex.width,
		.height = s.tex.height,
	};

	s.src = (Rectangle){
		.x = 0.f,
		.y = 0.f,
		.width = s.tex.width,
		.height = s.tex.height,
	};
	UnloadImage(img);
	return s;
}


SPRITE create_sprite_from_image_f(char* path, float w, float h)
{
	SPRITE s;
	s.path = malloc(strlen(path));
	strcpy(s.path, path);

	s.cosi = (Vector2){.x=0, .y=0};
	Image img = LoadImage(path);
	ImageResize(&
	img, w, h);
	s.tex = LoadTextureFromImage(img);

	// printf("SPRITE: %f %f\n", s.tex.width, s.tex.height);
	s.dst = (Rectangle){
		.x = 0,
		.y = 0,
		.width = w,
		.height = h,
	};

	s.src = (Rectangle){
		.x = 0,
		.y = 0,
		.width = w,
		.height = h,
	};
	UnloadImage(img);   // Once image has been converted to texture and uploaded to VRAM, it can be unloaded from RAM
	return s;
}

void unload_sprite(SPRITE* s)
{
	// UnloadImage(s->img);
	UnloadTexture(s->tex);
}

void draw_sprite_simple(SPRITE* s)
{
	// printf("Drawing image %f %f %f %f %f %f %f %f\n", s->dst.x, s->dst.y, s->dst.width, s->dst.height, s->src.width, s->src.height, s->tex.width, s->tex.height);
	// printf("Drawing image %f %f %f %f %f %f\n", s->dst.x, s->dst.y, s->dst.width, s->dst.height, s->tex.width, s->tex.height);
	DrawTexture(s->tex, s->dst.x, s->dst.y, WHITE);
}

void draw_sprite(SPRITE* s)
{
	// printf("Drawing image %f %f %f %f %f %f %f %f\n", s->dst.x, s->dst.y, s->dst.width, s->dst.height, s->src.width, s->src.height, s->tex.width, s->tex.height);
	DrawTexturePro(s->tex, s->src, s->dst, s->cosi, 0, WHITE);
}

#endif