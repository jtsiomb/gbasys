/*
Copyright 2004 John Tsiombikas <nuclear@siggraph.org>

This file is part of libgba, a library for GameBoy Advance development.

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

#ifndef _DMA_H_
#define _DMA_H_

void dma_copy32(int channel, void *dst, void *src, int words);
void dma_copy16(int channel, void *dst, void *src, int halfwords);

void dma_fill32(int channel, void *dst, unsigned long val, int words);
void dma_fill16(int channel, void *dst, unsigned short val, int halfwords);

#endif	/* _DMA_H_ */
