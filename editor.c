#include "editor.h"





void init_editor()
{
	SPRITE_VECTOR_INIT(&EDITOR_RENDER_QUEUE, 10);

	__selected_sprite_empty = calloc(1, sizeof(SPRITE));
	selected_sprite_ptr = calloc(1, sizeof(SPRITE));

	editor_frame = create_frame(1200, 800, &DEFAULT_FRAME_STYLE1);
	editor_frame->w_name = "EDITOR";
	bind_widget(editor_frame, "<MOUSE_MOVE>", update_selected_sprite_pos);
	bind_widget(editor_frame, "[MOUSE_BUTTON_LEFT]", place_sprite);

	add_widget_to_render_queue(editor_frame);

	EDITOR_INITALIZED = true;
}
