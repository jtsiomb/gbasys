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
#include "input.h"

volatile unsigned short *reg_key_state = (unsigned short*)0x4000130;
volatile unsigned short *reg_key_cntl = (unsigned short*)0x4000132;

int get_key_state(int key) {
	return ~(*reg_key_state) & key;
}

void enable_key_interrupts(int keys) {
	*reg_key_cntl |= (1 << 14) | keys;
}

void disable_key_interrupts(int keys) {
	if(keys == KEY_ALL) keys |= (1 << 14);
	*reg_key_cntl ^= keys;
}
