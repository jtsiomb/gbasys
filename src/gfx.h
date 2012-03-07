/*
This file is part of libgba, a library for GameBoy Advance development.
Copyright (C) 2004, 2005 John Tsiombikas <nuclear@siggraph.org>

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

#ifndef _GFX_H_
#define _GFX_H_

struct pixel_buffer {
	int x, y, bpp;
	void *pixels;
};

enum {
	VMODE_LFB_240x160_16	= 3,
	VMODE_LFB_240x160_8		= 4,
	VMODE_LFB_160x128_16	= 5
};

extern struct pixel_buffer *back_buffer, *front_buffer;

#define RGB(r, g, b)\
	((((b) >> 3) & 0x1f) << 10) |\
	((((g) >> 3) & 0x1f) << 5) |\
	(((r) >> 3) & 0x1f)

#define GET_R(c)	((((c) >> 10) & 0x1f) << 3)
#define GET_G(c)	((((c) >> 5) & 0x1f) << 3)
#define GET_B(c)	(((c) & 0x1f) << 3)

int set_video_mode(int mode, int back_buffering);
void flip(void);

struct pixel_buffer *create_pixel_buffer(int x, int y, int bpp);
void destroy_pixel_buffer(struct pixel_buffer *pbuf);

void clear_buffer(struct pixel_buffer *pbuf, unsigned short color);

void copy_buffer(const struct pixel_buffer *src, struct pixel_buffer *dst);

#define wait_vsync()	while(*((volatile unsigned short*)0x4000006) < front_buffer->y)

#define put_pixel(px, py, col, pbuf)	((unsigned short*)(pbuf)->pixels)[(py) * (pbuf)->x + (px)] = col
void draw_line(int x1, int y1, int x2, int y2, unsigned short col, struct pixel_buffer *pbuf);

#endif	/* _GFX_H_ */
