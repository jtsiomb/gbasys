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

#include "input.h"

#define REG_KEYSTATE	(*(unsigned short*)0x4000130)
#define REG_KEYCTL		(*(unsigned short*)0x4000132)

int get_key_state(int key) {
	int res;
	unsigned short prev_mask;

	prev_mask = REG_KEYCTL;
	disable_key_interrupts(key);

	res = ~REG_KEYSTATE & key;
	REG_KEYCTL = prev_mask;
	return res;
}

void enable_key_interrupts(int keys) {
	REG_KEYCTL |= (1 << 14) | keys;
}

void disable_key_interrupts(int keys) {
	if(keys == KEY_ALL) keys |= (1 << 14);
	REG_KEYCTL ^= keys;
}
