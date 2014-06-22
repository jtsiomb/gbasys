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
#include "config.h"
#include <stdlib.h>
#include "font.h"
#include "gfx.h"

#if defined(CONFIG_FONT_8X16)
#define DEFAULT_FONT	&font_8x16
#elif defined(CONFIG_FONT_8X8)
#define DEFAULT_FONT	&font_8x8
#else	/* no font compiled in */
#define DEFAULT_FONT	0
#endif	/* CONFIG_FONT_8X16 */

/* active font */
static struct font *font = DEFAULT_FONT;

/* active color */
static unsigned short fg_color = 0xffff;
static unsigned short bg_color = 0;

/* transparent background */
static unsigned char bg_transparent = 1;

/* ---- state manipulation ---- */
void set_font(struct font *fnt) {
	font = fnt;
}

struct font *get_font(void) {
	return font;
}

void set_text_color(unsigned short fg, unsigned short bg) {
	fg_color = fg;
	bg_color = bg;
}

void set_text_writebg(int enable) {
	bg_transparent = !enable;
}

struct pixel_buffer *get_glyph(unsigned char c, unsigned short fg, unsigned short bg, int bpp) {
	static struct pixel_buffer glyph;

	if(glyph.x != font->x || glyph.y != font->y || glyph.bpp != bpp) {
		glyph.x = font->x;
		glyph.y = font->y;
		glyph.bpp = bpp;
		free(glyph.pixels);
		if(!(glyph.pixels = malloc(glyph.x * glyph.y * (bpp >> 3)))) {
			return 0;
		}
	}

	if(bpp == 16) {
		int i, j;
		unsigned short *dptr = glyph.pixels;
		const unsigned char *bit_ptr = (unsigned char*)font->bitmap + font->y * (int)c;
		int sz = glyph.x * glyph.y;

		for(i=0, j=0; i<sz; i++) {
			*dptr++ = (*bit_ptr & (0x80 >> j++)) ? fg : bg;

			if(j > 7) {
				j = 0;
				bit_ptr++;
			}
		}

		return &glyph;
	}

	return 0;
}

int draw_glyph(unsigned char c, int x, int y, struct pixel_buffer *pbuf) {
	struct pixel_buffer *glyph;

	if(!(glyph = get_glyph(c, fg_color, bg_color, pbuf->bpp))) return -1;

	if(pbuf->bpp == 16) {
		int i, j;
		unsigned short *dptr = (unsigned short*)pbuf->pixels + y * pbuf->x + x;
		unsigned short *sptr = glyph->pixels;
		int advance = pbuf->x - glyph->x;

		for(j=0; j<glyph->y; j++) {
			for(i=0; i<glyph->x; i++) {
				if(bg_transparent && *sptr == bg_color) {
					dptr++;
					sptr++;
				} else {
					*dptr++ = *sptr++;
				}
			}
			dptr += advance;
		}
	}

	return 0;
}

/* low level string rendering */
int draw_string(const char *str, int x, int y, struct pixel_buffer *pbuf) {
	while(*str) {
		if(draw_glyph(*str++, x, y, pbuf) == -1) return -1;
		x += font->x;
	}
	return 0;
}
