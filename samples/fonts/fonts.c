#include "libgba.h"

int main(void) {
	int i;
	struct font *font;

	set_video_mode(VMODE_LFB_240x160_16, 1);

	font = get_font();

	clear_buffer(front_buffer, 0);
	draw_string("int main(void) {", 0, 0, front_buffer);
	draw_string("    return 0;", 0, font->y, front_buffer);
	draw_string("}", 0, font->y * 2, front_buffer);
	/*flip();*/

	halt();

	return 0;
}
