#include <raylib.h>
#include <raymath.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <dirent.h> 

#include "assets.h"
#include "util.c"
#include "sprite.h"


SPRITE_VECTOR* load_all_assets(const char* asset_path)
{
	SPRITE_VECTOR* list = malloc(sizeof(SPRITE_VECTOR));
	SPRITE_VECTOR_INIT(list, 10);
	if (asset_path == NULL) asset_path = "assets/";
	struct dirent* file;
	DIR* dir =  opendir(asset_path);
	if (dir == NULL) 
	{
		ERROR_OUT("Couldn't open directory %s", asset_path);
		return NULL;
	}

	 while ((file = readdir(dir)) != NULL) 
			printf("%s\n", file->d_name); 
  
	closedir(dir);	 
	return list;
}

void unload_all_assets(SPRITE_VECTOR* l)
{
	SPRITE_VECTOR ll = *l;
	ITERATE_VECTOR(ll, SPRITE, val)
	{
		unload_sprite(val);
	}
}