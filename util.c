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

#define TEST(a, b) {something or the other}


Vector2 get_relative_pos_PointRec(Vector2 p, Rectangle r)
{
	return (Vector2){.x = abs(r.x-p.x), .y = abs(r.y-p.y)};
}




#endif