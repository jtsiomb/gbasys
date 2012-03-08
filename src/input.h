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

#ifndef _INPUT_H_
#define _INPUT_H_

enum {
	KEY_A		= 1,
	KEY_B		= 2,
	KEY_SELECT	= 4,
	KEY_START	= 8,
	KEY_RIGHT	= 16,
	KEY_LEFT	= 32,
	KEY_UP		= 64,
	KEY_DOWN	= 128,
	KEY_R		= 256,
	KEY_L		= 512,
	KEY_ALL		= 1023
};

#define KEY_COUNT	14

extern volatile unsigned short *reg_key_state;

int get_key_state(int key);
void enable_key_interrupts(int keys);
void disable_key_interrupts(int keys);

#endif	/* _INPUT_H_ */
