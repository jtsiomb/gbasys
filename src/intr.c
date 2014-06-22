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
#include "intr.h"
#include "error.h"

#include "gfx.h"

#define MAX_INTR	14
static void (*intr_table[MAX_INTR])(void);

static void unexpected_intr(void) {
	panic("unexpected interrupt");
}

static void intr_handler(void) {
	int i;
	unsigned short irq;
	char buf[20];

	clr_int();

	irq = REG_INTR & 0x3fff;

	for(i=0; i<MAX_INTR; i++) {
		unsigned short irq_bit = (1 << i);
		if((irq & irq_bit) && intr_table[i]) {
			intr_table[i]();
		}
	}

	REG_INTR = irq;

	set_int();
}

void intr_init(void) {
	int i;
	unsigned long *ptr = (unsigned long*)0x3007ffc;
	*ptr = (unsigned long)intr_handler;

	for(i=0; i<MAX_INTR; i++) {
		interrupt(i, unexpected_intr);
	}
}

void interrupt(int intr, void (*handler)(void)) {
	intr_table[intr] = handler;
}
