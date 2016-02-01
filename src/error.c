/*
gbasys - a gameboy advance hardware abstraction library
Copyright (C) 2004-2016  John Tsiombikas <nuclear@member.fsf.org>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#include <stdio.h>
#include <stdarg.h>
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

void panicf(const char *fmt, ...)
{
	static char buf[64];
	va_list ap;

	va_start(ap, fmt);
	vsnprintf(buf, sizeof buf - 1, fmt, ap);
	va_end(ap);

	panic(buf);
}
