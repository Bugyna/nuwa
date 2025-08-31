#include <raylib.h>
#include <raymath.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#include "util.c"
#include "sprite.h"
#include "arena.h"
#include "animation.h"

#ifndef SCENE_H_
#define SCENE_H_

typedef struct SCENE SCENE;


struct SCENE
{
	LAYER_VECTOR layers;
};

DEFINE_VECTOR(SCENE_VECTOR, SCENE);

#endif


