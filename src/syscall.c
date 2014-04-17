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

#include <stdio.h>
#include <stdarg.h>
#include "syscall.h"

void halt(void)
{
	__syscall(2);
}

void stop(void)
{
	__syscall(3);
}

void print_vba(const char *str, ...)
{
	char buf[128];
	va_list arg_list;

	va_start(arg_list, str);
	vsnprintf(buf, 128, str, arg_list);
	va_end(arg_list);

	__asm__ __volatile__(
		"mov r0, %0\n\t"
		"swi 0xff0000\n\t" :
		: "r" (buf)
		: "r0"
	);
}
