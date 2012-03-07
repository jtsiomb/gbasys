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

#ifndef _TERM_H_
#define _TERM_H_

#include <stdio.h>

struct key_node {
	int key;
	struct key_node *next;
};

void term_init(void);
int gba_getc(FILE *fp);
int gba_putc(int c, FILE *fp);

int gba_printf(const char *fmt, ...);

#ifdef getc
#undef getc
#endif
#define getc(fp)	gba_getc(fp)

#ifdef putc
#undef putc
#endif
#define putc(c, fp)	gba_putc(c, fp)

#ifdef printf
#undef printf
#endif
#define printf gba_printf

#endif	/* _TERM_H_ */
