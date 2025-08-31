#include <raylib.h>
#include <raymath.h>
#include <locale.h>


#include "core.h"
#include "assets.c"
#include "sprite.h"
#include "gui.c"
#include "eventloop.c"
#include "editor.c"
#include "animation.h"
#include "entity.h"
#include "scene.h"

#include <sys/resource.h>


#if defined(PLATFORM_DESKTOP)
	#define GLSL_VERSION			330
#else   // PLATFORM_ANDROID, PLATFORM_WEB
	#define GLSL_VERSION			100
#endif