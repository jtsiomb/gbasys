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
#include "signal.h"
#include "intr.h"

static void sig_invalid_handler(int signum) {
	panic("signal error");
}

static sighandler_t signal_handler[SIG_MAX];
static sighandler_t default_sig_handler[SIG_MAX];
static volatile int wait_for_signal;
static sighandler_t saved_signal;

void sig_init(void) {
	int i;

	for(i=0; i<SIG_MAX; i++) {
		default_sig_handler[i] = sig_invalid_handler;
	}

	default_sig_handler[SIGALRM] = SIG_IGN;
	default_sig_handler[SIGUSR1] = SIG_IGN;
	default_sig_handler[SIGUSR2] = SIG_IGN;
	default_sig_handler[SIGIO] = SIG_IGN;
	default_sig_handler[SIGTTIN] = SIG_IGN;
	for(i=0; i<SIG_MAX; i++) {
		signal_handler[i] = default_sig_handler[i];
	}
}

sighandler_t signal(int signum, sighandler_t handler) {
	sighandler_t prev = signal_handler[signum];

	signal_handler[signum] = handler == SIG_IGN ? 0 : (handler == SIG_DFL ? default_sig_handler[signum] : handler);

	return prev;
}

int raise(int signum) {
	if(signal_handler[signum] != SIG_IGN) {
		signal_handler[signum](signum);
		wait_for_signal = 0;
	}
	return 0;
}

int pause(void) {
	clr_int();
	wait_for_signal = 1;
	set_int();

	while(wait_for_signal);

	/*errno = EINTR;*/
	return -1;
}

void save_signal(int signum) {
	saved_signal = signal_handler[signum];
}

void restore_signal(int signum) {
	signal_handler[signum] = saved_signal;
}

sighandler_t signal_func(int signum)
{
	return signal_handler[signum];
}
