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
#include <stdlib.h>
#include "term.h"
#include "input.h"
#include "signal.h"
#include "intr.h"

static struct key_node *key_queue, *key_queue_tail;

static void key_handler(void);

void term_init(void) {
	mask(INTR_KEY);
	interrupt(INTR_KEY, key_handler);
	unmask(INTR_KEY);

	key_queue = malloc(sizeof *key_queue);
	key_queue->next = 0;
	key_queue_tail = key_queue;
}

static void noop(int sig) {}

int gba_getc(FILE *fp) {
	struct key_node *tmp;
	int c;

	if(fp != stdin) panic("getc: only stdin valid");

	save_signal(SIGTTIN);
	signal(SIGTTIN, noop);
	while(!key_queue->next) pause();
	restore_signal(SIGTTIN);

	tmp = key_queue;
	key_queue = key_queue->next;
	free(tmp);
	return key_queue->key;
}

static void key_handler(void) {
	int i, state;
	static unsigned long prev;
	unsigned long time;

	time = get_millisec();
	if(time - prev < 100) return;
	prev = time;

	state = get_key_state(KEY_ALL);
	for(i=0; i<KEY_COUNT; i++) {
		int bit = (1 << i);
		if(state & bit) {
			struct key_node *key = malloc(sizeof *key);
			key->key = bit;
			key->next = 0;
			key_queue_tail->next = key;
			key_queue_tail = key;
		}
	}
	raise(SIGTTIN);
}
