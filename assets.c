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


SPRITE_VECTOR load_all_assets(const char* asset_path)
{
	// SPRITE_VECTOR* list = malloc(sizeof(SPRITE_VECTOR));
	SPRITE_VECTOR list;
	SPRITE_VECTOR_INIT(&list, 10);
	if (asset_path == NULL) asset_path = "assets/";
	struct dirent* file;
	DIR* dir =  opendir(asset_path);
	if (dir == NULL) 
	{
		ERROR_OUT("Couldn't open directory %s", asset_path);
		return list;
	}

	char tmp[4096];
	strcpy(tmp, asset_path);
	size_t offset = strlen(asset_path);
	
	while ((file = readdir(dir)) != NULL) {
		if (strcmp(file->d_name, ".") == 0 || strcmp(file->d_name, "..") == 0 ) continue;
		strcpy(tmp+offset, file->d_name);
		// printf("%s\n", file->d_name); 
		printf("LOADING ASSET: %s\n", tmp);
		SPRITE s = create_sprite_from_image(tmp);
		SPRITE_VECTOR_ADD(&list, s);
	}
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