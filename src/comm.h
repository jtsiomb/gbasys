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
#ifndef COMM_H_
#define COMM_H_

enum {
	COMM_SIO8_MASTER,
	COMM_SIO8_SLAVE,
	COMM_SIO32_MASTER,
	COMM_SIO32_SLAVE,
	COMM_UART,
	COMM_GPIO
};

void comm_setup(int mode);

/* synchronous transfer, blocks until complete */
void sio_transfer(void *in, const void *out);

/* async transfer, returns immediately, raises SIGIO when complete */
void sio_transfer_async(void *in, const void *out);


/* GPIO functions */
enum { GPIO_DIR_IN, GPIO_DIR_OUT };

void gpio_dir(int dir_so, int dir_si, int dir_sd, int dir_sc);
void gpio_dir_mask(unsigned char dir);

void gpio_set(unsigned char val);
unsigned char gpio_get(void);

#endif	/* COMM_H_ */
