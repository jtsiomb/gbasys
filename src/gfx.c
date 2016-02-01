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
#include <string.h>
#include "gfx.h"

#define FRAME_SEL_BIT	0x10

#define H_BLANK_OAM		0x20
#define OBJ_MAP_2D		0x0
#define OBJ_MAP_1D		0x40
#define FORCE_BLANK		0x80
#define BG0_ENABLE		0x100
#define BG1_ENABLE		0x200
#define BG2_ENABLE		0x400
#define BG3_ENABLE		0x800

#define REG_DISPCTL		(*(unsigned short*)0x4000000)
#define REG_VCOUNT		(*(unsigned short*)0x4000006)

#define REG_BG2PA		(*(short*)0x4000020)
#define REG_BG2PB		(*(short*)0x4000022)
#define REG_BG2PC		(*(short*)0x4000024)
#define REG_BG2PD		(*(short*)0x4000026)

static unsigned short *paladdr = (void*)0x5000000;

static int xres, yres;
static int sizeof_pixel;
static int page_flipping;
static int allocated_back_buffer;

static struct pixel_buffer fbuf, bbuf;
struct pixel_buffer *front_buffer = &fbuf;
struct pixel_buffer *back_buffer = &bbuf;

#define show_page(n)	((n) ? (REG_DISPCTL |= FRAME_SEL_BIT) : (REG_DISPCTL &= ~FRAME_SEL_BIT))
#define swap_page()		(REG_DISPCTL ^= FRAME_SEL_BIT)

int set_video_mode(int mode, int double_buffering) {
	if(mode < 3 || mode > 5) return -1;

	/* mode 5: 160x128, otherwise: 240x160 */
	xres = (mode == 5) ? 160 : 240;
	yres = (mode == 5) ? 128 : 160;

	sizeof_pixel = (mode == 4) ? 1 : 2;

	REG_DISPCTL = mode | BG2_ENABLE;

	show_page(0);

	if(allocated_back_buffer) {
		free(back_buffer->pixels);
	}

	front_buffer->pixels = (void*)0x6000000;
	front_buffer->x = back_buffer->x = xres;
	front_buffer->y = back_buffer->y = yres;
	front_buffer->bpp = back_buffer->bpp = sizeof_pixel * 8;

	if(mode > 3) {
		page_flipping = 1;
		back_buffer->pixels = (void*)0x600a000;
	} else {
		page_flipping = 0;
		if(double_buffering) {
			back_buffer->pixels = malloc(xres * yres * sizeof_pixel);
		}
	}
}

void flip(void) {
	static void *tmp;

	if(page_flipping) {
		swap_page();
		tmp = front_buffer->pixels;
		front_buffer->pixels = back_buffer->pixels;
		back_buffer->pixels = tmp;
	} else {
		/*dma_copy32(3, front_buffer->pixels, back_buffer->pixels, (xres * yres) >> 1);*/
		dma_copy32(3, front_buffer->pixels, back_buffer->pixels, 19200);
	}
}

/* ------- pixel buffer operations ------- */

struct pixel_buffer *create_pixel_buffer(int x, int y, int bpp) {
	struct pixel_buffer *pbuf = malloc(sizeof(struct pixel_buffer));
	if(pbuf) {
		pbuf->pixels = malloc(x * y * bpp / 8);
		pbuf->x = x;
		pbuf->y = y;
		pbuf->bpp = bpp;
	}
	return pbuf;
}

void destroy_pixel_buffer(struct pixel_buffer *pbuf) {
	free(pbuf->pixels);
	free(pbuf);
}

void clear_buffer(struct pixel_buffer *pbuf, unsigned short color) {
	int sz = pbuf->x * pbuf->y;

	if(pbuf->bpp == 8) {
		color |= color << 8;
		sz >>= 1;
	}

	dma_fill16(3, pbuf->pixels, color, sz);
}

void copy_buffer(const struct pixel_buffer *src, struct pixel_buffer *dst) {
	int words;

	if(src->x != dst->x || src->y != dst->y || src->bpp != dst->bpp) return;

	words = (src->x * src->y) >> (src->bpp == 16 ? 1 : 2);
	dma_copy32(3, dst->pixels, src->pixels, words);
}

#define MIN(a, b)	((a) < (b) ? (a) : (b))

void blit(struct pixel_buffer *src, int src_x, int src_y, int src_w, int src_h,
		struct pixel_buffer *dst, int dst_x, int dst_y)
{
	int i, pixsize, width, height, dstride, sstride;
	unsigned char *dptr, *sptr;

	if(dst->bpp != src->bpp)
		return;

	if(src_w <= 0)
		src_w = src->x;
	if(src_h <= 0)
		src_h = src->y;

	width = MIN(src_w, MIN(src->x - src_x, dst->x - dst_x));
	height = MIN(src_h, MIN(src->y - src_y, dst->y - dst_y));

	if(width <= 0 || height <= 0)
		return;

	pixsize = dst->bpp / 8;
	dptr = (unsigned char*)dst->pixels + (dst_y * dst->x + dst_x) * pixsize;
	sptr = (unsigned char*)src->pixels + (src_y * src->x + src_x) * pixsize;

	dstride = dst->x * pixsize;
	sstride = src->x * pixsize;

	for(i=0; i<height; i++) {
		memcpy(dptr, sptr, width * pixsize);
		sptr += sstride;
		dptr += dstride;
	}
}

void set_palette(int idx, int r, int g, int b)
{
	paladdr[idx] = RGB(r, g, b);
}

void set_bg_matrix(int a, int b, int c, int d)
{
	REG_BG2PA = a;
	REG_BG2PB = b;
	REG_BG2PC = c;
	REG_BG2PD = d;
}

void set_bg_scale(int x, int y)
{
	set_bg_matrix(x, 0, 0, y);
}

void draw_line(int x1, int y1, int x2, int y2, unsigned short col, struct pixel_buffer *pbuf) {
	int dx, dy, dx2, dy2;
	int x_inc, y_inc;
	int error;
	int i;
	unsigned short *ptr = pbuf->pixels;

	ptr += y1 * pbuf->x + x1;
	dx = x2 - x1;
	dy = y2 - y1;

	if(dx >= 0) {
		x_inc = 1;
	} else {
		x_inc = -1;
		dx = -dx;
	}

	if(dy >= 0) {
		y_inc = pbuf->x;
	} else {
		y_inc = -pbuf->x;
		dy = -dy;
	}

	dx2 = dx << 1;
	dy2 = dy << 1;

	if(dx > dy) {
		error = dy2 - dx;
		for(i=0; i<=dx; i++) {
			*ptr = col;
			if(error >= 0) {
				error -= dx2;
				ptr += y_inc;
			}
			error += dy2;
			ptr += x_inc;
		}
	} else {
		error = dx2 - dy;
		for(i=0;i<=dy;i++) {
			*ptr = col;
			if(error >= 0) {
				error -= dy2;
				ptr += x_inc;
			}
			error += dx2;
			ptr += y_inc;
		}
	}
}

enum {
	OUT_L = 1,
	OUT_R = 2,
	OUT_T = 4,
	OUT_B = 8
};
static unsigned int outcode(int x, int y, int left, int top, int right, int bot)
{
	unsigned int res = 0;
	if(x < left) res |= OUT_L;
	if(x > right) res |= OUT_R;
	if(y < top) res |= OUT_T;
	if(y > bot) res |= OUT_B;
	return res;
}

int clip_line(int *x1, int *y1, int *x2, int *y2, int left, int top, int right, int bot)
{
	int i;
	unsigned int out1, out2;

	--right;
	--bot;
	out1 = outcode(*x1, *y1, left, top, right, bot);
	out2 = outcode(*x2, *y2, left, top, right, bot);

	for(;;) {
		int x, y;
		unsigned int outout;

		if((out1 | out2) == 0)
			return 1;	/* both inside */
		if(out1 & out2)
			return 0;	/* both outside in the same area */

		outout = out1 ? out1 : out2;
		if(outout & OUT_T) {
			x = *x1 + (float)(*x2 - *x1) * (float)(top - *y1) / (float)(*y2 - *y1);
			y = top;
		} else if(outout & OUT_B) {
			x = *x1 + (float)(*x2 - *x1) * (float)(bot - *y1) / (float)(*y2 - *y1);
			y = bot;
		} else if(outout & OUT_R) {
			y = *y1 + (float)(*y2 - *y1) * (float)(right - *x1) / (float)(*x2 - *x1);
			x = right;
		} else if(outout & OUT_L) {
			y = *y1 + (float)(*y2 - *y1) * (float)(left - *x1) / (float)(*x2 - *x1);
			x = left;
		}

		if(out1) {
			*x1 = x;
			*y1 = y;
			out1 = outcode(*x1, *y1, left, top, right, bot);
		} else {
			*x2 = x;
			*y2 = y;
			out2 = outcode(*x2, *y2, left, top, right, bot);
		}
	}
	return 1;
}
