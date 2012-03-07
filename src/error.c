/*
Copyright 2004-2012 John Tsiombikas <nuclear@member.fsf.org>

This file is part of gbasys, a library for GameBoy Advance development.

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

#include "gfx.h"

void panic(const char *str) {
	set_video_mode(VMODE_LFB_240x160_16, 0);
	clear_buffer(front_buffer, RGB(140, 30, 20));

	set_text_color(0xffff, RGB(140, 30, 20));
	draw_string(" Don't Panic! ", 0, 0, front_buffer);
	draw_string("_________________________", 0, 8, front_buffer);
	if(str) draw_string(str, 0, 32, front_buffer);

	for(;;);
}
