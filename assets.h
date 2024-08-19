#include <raylib.h>
#include <raymath.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>


#include "util.c"
#include "sprite.h"

#ifndef ASSETS_H_
#define ASSETS_H_


SPRITE_VECTOR* load_all_assets(const char*);
void unload_all_assets(SPRITE_VECTOR*);

#endif