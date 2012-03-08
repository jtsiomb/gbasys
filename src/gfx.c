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

static volatile unsigned short *reg_disp_ctl = (void*)0x4000000;
static volatile unsigned short *reg_vcount = (void*)0x4000006;

static int xres, yres;
static int sizeof_pixel;
static int page_flipping;
static int allocated_back_buffer;

static struct pixel_buffer fbuf, bbuf;
struct pixel_buffer *front_buffer = &fbuf;
struct pixel_buffer *back_buffer = &bbuf;

#define show_page(n)	((n) ? (*reg_disp_ctl |= FRAME_SEL_BIT) : (*reg_disp_ctl &= ~FRAME_SEL_BIT))
#define swap_page()		(*reg_disp_ctl ^= FRAME_SEL_BIT)

int set_video_mode(int mode, int double_buffering) {
	if(mode < 3 || mode > 5) return -1;

	/* mode 5: 160x128, otherwise: 240x160 */
	xres = (mode == 5) ? 160 : 240;
	yres = (mode == 5) ? 128 : 160;

	sizeof_pixel = (mode == 4) ? 1 : 2;

	*reg_disp_ctl = mode | BG2_ENABLE;

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
