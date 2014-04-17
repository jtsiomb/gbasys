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
#include "dma.h"

/* DMA Options */
#define DMA_ENABLE				0x80000000
#define DMA_INT_ENABLE			0x40000000
#define DMA_TIMING_IMMED		0x00000000
#define DMA_TIMING_VBLANK		0x10000000
#define DMA_TIMING_HBLANK		0x20000000
#define DMA_TIMING_DISPSYNC		0x30000000
#define DMA_16					0x00000000
#define DMA_32					0x04000000
#define DMA_REPEAT				0x02000000
#define DMA_SRC_INC				0x00000000
#define DMA_SRC_DEC				0x00800000
#define DMA_SRC_FIX				0x01000000
#define DMA_DST_INC				0x00000000
#define DMA_DST_DEC				0x00200000
#define DMA_DST_FIX1			0x00400000
#define DMA_DST_RELOAD			0x00600000

/* DMA Register Parts */
#define DMA_SRC		0
#define DMA_DST		1
#define DMA_CTRL	2

static volatile unsigned long *reg_dma[4] = {(void*)0x040000b0, (void*)0x040000bc, (void*)0x040000c8, (void*)0x040000d4 };

/* --- perform a copy of words or halfwords using DMA --- */

void dma_copy32(int channel, void *dst, void *src, int words)
{
	reg_dma[channel][DMA_SRC] = (unsigned long)src;
	reg_dma[channel][DMA_DST] = (unsigned long)dst;
	reg_dma[channel][DMA_CTRL] = words | DMA_TIMING_IMMED | DMA_32 | DMA_ENABLE;
}

void dma_copy16(int channel, void *dst, void *src, int halfwords)
{
	reg_dma[channel][DMA_SRC] = (unsigned long)src;
	reg_dma[channel][DMA_DST] = (unsigned long)dst;
	reg_dma[channel][DMA_CTRL] = halfwords | DMA_TIMING_IMMED | DMA_16 | DMA_ENABLE;
}

/* --- fill a buffer with an ammount of words and halfwords using DMA --- */

void dma_fill32(int channel, void *dst, unsigned long val, int words)
{
	unsigned long valmem = val;
	reg_dma[channel][DMA_SRC] = (unsigned long)&valmem;
	reg_dma[channel][DMA_DST] = (unsigned long)dst;
	reg_dma[channel][DMA_CTRL] = words | DMA_SRC_FIX | DMA_TIMING_IMMED | DMA_32 | DMA_ENABLE;
}

void dma_fill16(int channel, void *dst, unsigned short val, int halfwords)
{
	unsigned short valmem = val;
	reg_dma[channel][DMA_SRC] = (unsigned long)&valmem;
	reg_dma[channel][DMA_DST] = (unsigned long)dst;
	reg_dma[channel][DMA_CTRL] = halfwords | DMA_SRC_FIX | DMA_TIMING_IMMED | DMA_16 | DMA_ENABLE;
}
