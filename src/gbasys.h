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
#ifndef _LIBGBA_H_
#define _LIBGBA_H_

#include "gfx.h"
#include "dma.h"
#include "syscall.h"
#include "font.h"
#include "input.h"
#include "intr.h"
#include "error.h"
#include "signal.h"
#include "timer.h"
#include "term.h"
#include "comm.h"

void gba_init(void);

#endif	/* _LIBGBA_H_ */
