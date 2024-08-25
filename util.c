#include <raylib.h>
#include <raymath.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#ifndef UTIL_H_
#define UTIL_H_
#include "SLL/linked_list.h"
#include "SHM/hashmap.h"


#define ERROR_OUT(err, ...) {printf("ERROR: " err"\n", __VA_ARGS__); exit(-33);}
#define NUWA_ERROR(err, ...) {printf("NUWA ERROR: "); printf(err __VA_ARGS__); exit(-33);}
#define NUWA_ERROR_N(err, ...) {printf("NUWA ERROR: "); printf(err, __VA_ARGS__); exit(-33);}

#define TEST(a, b) {something or the other}


Vector2 get_relative_pos_PointRec(Vector2 p, Rectangle r)
{
	return (Vector2){.x = abs(r.x-p.x), .y = abs(r.y-p.y)};
}



void draw_grid(Rectangle pos, int size, Color fg)
{
	int y = 0;
	for (int x = 0; x <= pos.width; x += size) {
		DrawLine(x+pos.x, pos.y, x+pos.x, pos.y+pos.height, fg);
		for (y; y <= pos.height; y += size) {
			DrawLine(pos.x+x, y+pos.y, pos.x+pos.width, y+pos.y, fg);
		}
	}
}

void draw_grid_bg(Rectangle pos, int size, Color bg, Color fg)
{
	DrawRectangleRec(pos, bg);
	int y = 0;
	for (int x = 0; x <= pos.width; x += size) {
		DrawLine(x+pos.x, pos.y, x+pos.x, pos.y+pos.height, fg);
		for (y; y <= pos.height; y += size) {
			DrawLine(pos.x+x, y+pos.y, pos.x+pos.width, y+pos.y, fg);
		}
	}
}



#endif