#include <raylib.h>
#include <raymath.h>
#include <locale.h>

#include "assets.c"
#include "sprite.h"
#include "gui.c"
#include "eventloop.c"
#include "editor.c"
#include <sys/resource.h>


#if defined(PLATFORM_DESKTOP)
	#define GLSL_VERSION			330
#else   // PLATFORM_ANDROID, PLATFORM_WEB
	#define GLSL_VERSION			100
#endif

#define MAX_POSTPRO_SHADERS		 12


typedef enum {
	FX_GRAYSCALE = 0,
	FX_POSTERIZATION,
	FX_DREAM_VISION,
	FX_PIXELIZER,
	FX_CROSS_HATCHING,
	FX_CROSS_STITCHING,
	FX_PREDATOR_VIEW,
	FX_SCANLINES,
	FX_FISHEYE,
	FX_SOBEL,
	FX_BLOOM,
	FX_BLUR,
	//FX_FXAA
} PostproShader;

static const char *postproShaderText[] = {
	"GRAYSCALE",
	"POSTERIZATION",
	"DREAM_VISION",
	"PIXELIZER",
	"CROSS_HATCHING",
	"CROSS_STITCHING",
	"PREDATOR_VIEW",
	"SCANLINES",
	"FISHEYE",
	"SOBEL",
	"BLOOM",
	"BLUR",
	//"FXAA"
};

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------


int main(void)
{
	setlocale(LC_ALL, "");
	// Initialization
	//--------------------------------------------------------------------------------------
	const int screenWidth = 1200;
	const int screenHeight = 960;

	SetConfigFlags(FLAG_MSAA_4X_HINT);	  // Enable Multi Sampling Anti Aliasing 4x (if available)
	// SetConfigFlags(FLAG_FULLSCREEN_MODE);
	SetConfigFlags(FLAG_WINDOW_RESIZABLE);

	InitWindow(screenWidth, screenHeight, "raylib [shaders] example - postprocessing shader");

	init_gui_system(screenWidth, screenHeight);
	// SetExitKey(0); 

	// Define the camera to look into our 3d world
	Camera camera = { 0 };
	camera.position = (Vector3){ 2.0f, 3.0f, 1.0f };	// Camera position
	// camera.position = (Vector3){ 2.0f, 13.0f, 12.0f };	// Camera position
	camera.target = (Vector3){ 0.0f, 10.0f, 0.0f };	  // Camera looking at point
	camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };		  // Camera up vector (rotation towards target)
	camera.fovy = 80.0f;								// Camera field-of-view Y
	camera.projection = CAMERA_PERSPECTIVE;			 // Camera projection type

	// Model model = LoadModel("alzak.gltf");				 // Load OBJ model
	// Model model = LoadModel("ahri/Ahri Star Guardian.obj");				 // Load OBJ model
	// Model model = LoadModel("Hatsune Miku/mikitm500/mikitm500_obj.obj");				 // Load OBJ model
	// Model model = LoadModel("Hatsune Miku/mikitm000/mikitm000_obj.obj");				 // Load OBJ model
	// Texture2D texture = LoadTexture("ahri/Ahri_Skin14_TX_CM.png"); // Load model texture (diffuse map)
	// Texture2D texture1 = LoadTexture("ahri/Ahri_Skin14_TAILS_TX_CM.png"); // Load model texture (diffuse map)
	// Texture2D texture2 = LoadTexture("ahri/Ahri_Skin14_Familiar_TX_CM.png"); // Load model texture (diffuse map)
	// model.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = texture;		// Set model diffuse texture
	// model.materials[1].maps[MATERIAL_MAP_DIFFUSE].texture = texture1;		// Set model diffuse texture
	// model.materials[2].maps[MATERIAL_MAP_DIFFUSE].texture = texture2;		// Set model diffuse texture

	Vector3 position = { 1.5f, 2.5f, 1.7f };			// Set model position
	// Vector3 position = { 1.5f, 2.5f, 1.7f };			// Set model position

	// Load all postpro shaders
	// NOTE 1: All postpro shader use the base vertex shader (DEFAULT_VERTEX_SHADER)
	// NOTE 2: We load the correct shader depending on GLSL version
	Shader shaders[MAX_POSTPRO_SHADERS] = { 0 };
	// NOTE: Defining 0 (NULL) for vertex shader forces usage of internal default vertex shader
	shaders[FX_GRAYSCALE] = LoadShader(0, TextFormat("shaders/glsl%i/grayscale.fs", GLSL_VERSION));
	shaders[FX_POSTERIZATION] = LoadShader(0, TextFormat("shaders/glsl%i/posterization.fs", GLSL_VERSION));
	shaders[FX_DREAM_VISION] = LoadShader(0, TextFormat("shaders/glsl%i/dream_vision.fs", GLSL_VERSION));
	shaders[FX_PIXELIZER] = LoadShader(0, TextFormat("shaders/glsl%i/pixelizer.fs", GLSL_VERSION));
	shaders[FX_CROSS_HATCHING] = LoadShader(0, TextFormat("shaders/glsl%i/cross_hatching.fs", GLSL_VERSION));
	shaders[FX_CROSS_STITCHING] = LoadShader(0, TextFormat("shaders/glsl%i/cross_stitching.fs", GLSL_VERSION));
	shaders[FX_PREDATOR_VIEW] = LoadShader(0, TextFormat("shaders/glsl%i/predator.fs", GLSL_VERSION));
	shaders[FX_SCANLINES] = LoadShader(0, TextFormat("shaders/glsl%i/scanlines.fs", GLSL_VERSION));
	shaders[FX_FISHEYE] = LoadShader(0, TextFormat("shaders/glsl%i/fisheye.fs", GLSL_VERSION));
	shaders[FX_SOBEL] = LoadShader(0, TextFormat("shaders/glsl%i/sobel.fs", GLSL_VERSION));
	shaders[FX_BLOOM] = LoadShader(0, TextFormat("shaders/glsl%i/bloom.fs", GLSL_VERSION));
	shaders[FX_BLUR] = LoadShader(0, TextFormat("shaders/glsl%i/blur.fs", GLSL_VERSION));

	int currentShader = FX_GRAYSCALE;

	RenderTexture2D target = LoadRenderTexture(screenWidth, screenHeight);
	SPRITE_VECTOR* asset_list = load_all_assets(NULL);

	// Create a RenderTexture2D to be used for render to texture
	// Image wall_img = LoadImage("gg_wallpaper.png");			 // Load image in CPU memory (RAM)
	// ImageResize(&wall_img, 300, 300);
	// Texture2D wall_texture = LoadTextureFromImage(wall_img);          // Image converted to texture, GPU memory (VRAM)
	// UnloadImage(wall_img);   // Once image has been converted to texture and uploaded to VRAM, it can be unloaded from RAM

	// Image wall1_img = LoadImage("gg_wallpaper1.png");			 // Load image in CPU memory (RAM)
	// ImageResize(&wall1_img, 2048, 2048);
	// Texture2D wall1_texture = LoadTextureFromImage(wall1_img);          // Image converted to texture, GPU memory (VRAM)
	// UnloadImage(wall1_img);   // Once image has been converted to texture and uploaded to VRAM, it can be unloaded from RAM
	
	// Image test_img = LoadImage("pierce.png");			 // Load image in CPU memory (RAM)
	// // ImageResize(&test_img, , 300);
	// Texture2D test_texture = LoadTextureFromImage(test_img);          // Image converted to texture, GPU memory (VRAM)
	// UnloadImage(test_img);   // Once image has been converted to texture and uploaded to VRAM, it can be unloaded from RAM

	// Image cabinet = LoadImage("gg_cabinet.png");			 // Load image in CPU memory (RAM)
	// ImageResize(&cabinet, 300, 300);
	// Texture2D cabinet_texture = LoadTextureFromImage(cabinet);          // Image converted to texture, GPU memory (VRAM)
	// UnloadImage(cabinet);   // Once image has been converted to texture and uploaded to VRAM, it can be unloaded from RAM

	// Image painting = LoadImage("gg_painting.png");			 // Load image in CPU memory (RAM)
	// ImageResize(&painting, 150, 150);
	// Texture2D painting_texture = LoadTextureFromImage(painting);          // Image converted to texture, GPU memory (VRAM)
	// UnloadImage(painting);   // Once image has been converted to texture and uploaded to VRAM, it can be unloaded from RAM

	// Image bed = LoadImage("gg_bed.png");			 // Load image in CPU memory (RAM)
	// ImageResize(&bed, 200, 130);
	// Texture2D bed_texture = LoadTextureFromImage(bed);          // Image converted to texture, GPU memory (VRAM)
	// UnloadImage(bed);   // Once image has been converted to texture and uploaded to VRAM, it can be unloaded from RAM

	// SPRITE s;
	// s.tex = bed_texture;
	// s.src = (Rectangle){.x=0, .y=0, .width=100, .height=100};
	// s.dst = (Rectangle){.x=0, .y=0, .width=100, .height=100};
	SPRITE s = create_sprite_from_image("assets/gg_bed.png");

	SetTargetFPS(120);				   // Set our game to run at 60 frames-per-second
	//--------------------------------------------------------------------------------------

	WIDGET* test_frame = create_frame(400, 400, NULL);
	// WIDGET* test_child = create_child_widget(test_frame, 
	WIDGET* test_widget = create_widget(100, 100, W_LABEL, 200, 200, "test", NULL);
	WIDGET* test_widget1 = create_widget(100, 400, W_LABEL, 200, 200, "test1", NULL);
	WIDGET* test_widget2 = create_widget(100, 400, W_LABEL, 200, 200, "test2", NULL);
	WIDGET* test_widget3 = create_widget(100, 400, W_LABEL, 200, 200, "test3", NULL);
	WIDGET* test_widget4 = create_widget(100, 400, W_LABEL, 200, 200, "test4", NULL);
	WIDGET* test_widget5 = create_widget(100, 400, W_LABEL, 200, 200, "test5", NULL);


	WIDGET* test_dropdown = create_dropdown("select sumn", NULL);
	dropdown_add_option(test_dropdown, "sumn0");

	WIDGET* test_text_input = create_text_input("", NULL);

	add_child_widget(test_frame, test_widget);
	add_child_widget(test_frame, test_widget1);
	add_child_widget(test_frame, test_widget2);
	add_child_widget(test_frame, test_widget3);
	add_child_widget(test_frame, test_widget4);
	add_child_widget(test_frame, test_widget5);

	add_widget_to_render_queue(test_frame);
	add_widget_to_render_queue(test_dropdown);
	add_widget_to_render_queue(test_text_input);

	// SPRITE s = create_sprite_from_image_f("assets/gg_bed.png", 200, 200);
	// widget_add_img(test_widget, &s);
	test_widget->img = &s;
	// printf("bool: %d\n", &test_widget->img != &EMPTY_SPRITE);

	focus_set(test_text_input);
	// add_widget_to_render_queue(test_widget);
	// add_widget_to_render_queue(test_widget1);
	// add_widget_to_render_queue(test_widget2);
	// remove_widget_from_render_queue(test_widget);
	char text_pos_buf[100];
	// Main game loop
	while (!WindowShouldClose())		// Detect window close button or ESC key
	{
		// Update
		//----------------------------------------------------------------------------------
		// UpdateCamera(&camera, CAMERA_ORBITAL);

		if (IsKeyPressed(KEY_RIGHT)) currentShader++;
		else if (IsKeyPressed(KEY_LEFT)) currentShader--;

		if (currentShader >= MAX_POSTPRO_SHADERS) currentShader = 0;
		else if (currentShader < 0) currentShader = MAX_POSTPRO_SHADERS - 1;
		//----------------------------------------------------------------------------------
		event_handle();
		// Draw
		// //----------------------------------------------------------------------------------
		BeginTextureMode(target);	   // Enable drawing to texture
			ClearBackground(RAYWHITE);  // Clear texture background
			// DrawTexture(s.tex, 100, 200, WHITE);
			// // draw_widget(test_widget);
			// DrawTexture(wall_texture, 0, 100, WHITE);
			// // DrawTexture(wall1_texture, wall_texture.width, 100, WHITE);
			// DrawTexturePro(bed_texture, (Rectangle){0,0,2048, 2048}, (Rectangle){0, 0, 1200, 960}, (Vector2){0, 0}, 0, WHITE);
			// // DrawTexture(cabinet_texture, cabinet_texture.width, 400, WHITE);
			// // DrawTexture(painting_texture, 800, 100, WHITE);
			// DrawTexture(bed_texture, 500, 200, WHITE);
			// // DrawTexturePro(test_texture, (Rectangle){0,0,2048, 2048}, (Rectangle){200, 200, 400, 400}, (Vector2){0, 0}, 0, WHITE);
			
			// // DrawTextureRec(test_texture, (Rectangle){ 0, 0, 200, 300 }, (Vector2){ 0, 0 }, WHITE);
			// // BeginMode3D(camera);		// Begin 3d mode drawing
				// // DrawModel(model, position, 0.1f, WHITE);   // Draw 3d model with texture
				// // DrawGrid(10, 1.0f);	 // Draw a grid
			// // EndMode3D();				// End 3d mode drawing, returns to orthographic 2d mode
		EndTextureMode();			   // End drawing to texture (now we have a texture available for next passes)
		
		BeginDrawing();
			ClearBackground(RAYWHITE);  // Clear screen background
			// DrawTexture(test_texture, screenWidth/2 - texture.width/2, screenHeight/2 - texture.height/2, WHITE);
			
			// Render generated texture using selected postprocessing shader
			BeginShaderMode(shaders[currentShader]);
				// NOTE: Render texture must be y-flipped due to default OpenGL coordinates (left-bottom)
				DrawTextureRec(target.texture, (Rectangle){ 0, 0, (float)target.texture.width, (float)-target.texture.height }, (Vector2){ 0, 0 }, WHITE);
			EndShaderMode();

			// Draw 2d shapes and text over drawn texture
			DrawRectangle(0, 9, 580, 30, Fade(LIGHTGRAY, 0.7f));
			// draw_sprite(&s);

			DrawText("(c) nuwa", screenWidth - 200, screenHeight - 20, 10, GRAY);
			DrawText("CURRENT POSTPRO SHADER:", 10, 15, 20, BLACK);
			DrawText(postproShaderText[currentShader], 330, 15, 20, RED);
			DrawText("< >", 540, 10, 30, DARKBLUE);
			DrawFPS(700, 15);

			DrawText(__WIDGET_FOCUS->w_name, 500, 200, 20, BLACK);
			DrawText(__WIDGET_ATTENTION->w_name, 500, 240, 20, BLACK);
			DrawText(__WIDGET_LOCK->w_name, 500, 280, 20, BLACK);
			DrawText(__EVENT_ALL, 500, 300, 20, BLACK);
			sprintf(text_pos_buf, "[%d:%d]", (int)test_text_input->cursor.y, (int)test_text_input->cursor.x);
			DrawText(text_pos_buf, 500, 320, 20, BLACK);


			draw_gui();
			// DrawTexture(s.tex, 100, 200, WHITE);

			// draw_widget(test_widget);
			// draw_widget(test_widget1);
			// event_handle_keyboard();
			// event_handle_mouse();
		EndDrawing();
		//----------------------------------------------------------------------------------
	}

	// De-Initialization
	//--------------------------------------------------------------------------------------
	// Unload all postpro shaders
	for (int i = 0; i < MAX_POSTPRO_SHADERS; i++) UnloadShader(shaders[i]);

	// UnloadTexture(test_texture);		 // Unload texture
	// UnloadTexture(texture);		 // Unload texture
	unload_sprite(&s);
	unload_all_assets(asset_list);
	// UnloadModel(model);			 // Unload model
	UnloadRenderTexture(target);	// Unload render texture

	CloseWindow();				  // Close window and OpenGL context
	//--------------------------------------------------------------------------------------

	return 0;
}