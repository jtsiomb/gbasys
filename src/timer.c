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
#include <limits.h>
#include "intr.h"
#include "signal.h"

/* prescalar selection based on the system clock (16.78MHz) */
#define TIMER_CNTL_CLK		0
#define TIMER_CNTL_CLK64	1
#define TIMER_CNTL_CLK256	2
#define TIMER_CNTL_CLK1024	3

/* control bits */
#define TIMER_CNTL_COUNTUP	4
#define TIMER_CNTL_INTR		0x40
#define TIMER_CNTL_ENABLE	0x80

static void timer_intr_handler(void);

volatile static unsigned short *reg_timer[] = {(void*)0x4000100, (void*)0x4000104, (void*)0x4000108, (void*)0x400010c};
static unsigned short *reg_timer_cntl[] = {(void*)0x4000102, (void*)0x4000106, (void*)0x400010a, (void*)0x400010e};

static unsigned long milli_sec;
static unsigned long alarm_val;

void enable_timer(int timer) {
	*reg_timer_cntl[timer] |= TIMER_CNTL_ENABLE;
}

void disable_timer(int timer) {
	*reg_timer_cntl[timer] &= ~TIMER_CNTL_ENABLE;
}

void reset_msec_timer(void) {
	*reg_timer_cntl[0] &= ~TIMER_CNTL_ENABLE;
	interrupt(INTR_TIMER0, timer_intr_handler);
	milli_sec = 0;
	*reg_timer[0] = USHRT_MAX - 16779;
	*reg_timer_cntl[0] = TIMER_CNTL_INTR | TIMER_CNTL_ENABLE;
	unmask(INTR_TIMER0);
}


unsigned long get_millisec(void) {
	return milli_sec;
}

unsigned int alarm(unsigned int seconds) {
	unsigned int prev = alarm_val;
	alarm_val = seconds * 1000;
}

static void timer_intr_handler(void) {
	milli_sec++;

	if(alarm_val > 0) {
		if(!--alarm_val) raise(SIGALRM);
	}
}
