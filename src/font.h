/*
gbasys - a gameboy advance hardware abstraction library
Copyright (C) 2004-2014  John Tsiombikas <nuclear@member.fsf.org>

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
#ifndef _FONT_H_
#define _FONT_H_

#include "config.h"

struct pixel_buffer;

struct font {
	int x, y;
	void *bitmap;
};


#ifdef CONFIG_FONT_8X8
extern struct font font_8x8;
#endif	/* CONFIG_FONT_8X8 */

#ifdef CONFIG_FONT_8X16
extern struct font font_8x16;
#endif	/* CONFIG_FONT_8X16 */


void set_font(struct font *fnt);
struct font *get_font(void);
void set_text_color(unsigned short fg, unsigned short bg);
void set_text_writebg(int enable);

struct pixel_buffer *get_glyph(unsigned char c, unsigned short fg, unsigned short bg, int bpp);
int draw_glyph(unsigned char c, int x, int y, struct pixel_buffer *pbuf);
int draw_string(const char *str, int x, int y, struct pixel_buffer *pbuf);


#endif	/* _FONT_H_ */
