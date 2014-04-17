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
#include "comm.h"
#include "error.h"
#include "signal.h"
#include "intr.h"

#define REG_SIODATA32	*((volatile unsigned short*)0x4000120)
#define REG_SIOCNT		*((volatile unsigned short*)0x4000128)
#define REG_SIODATA8	*((volatile unsigned short*)0x400012a)
#define REG_RCNT		*((volatile unsigned short*)0x4000134)

/* REG_SIOCNT bits */
#define SIOCNT_CLK_INTERN		(1 << 0)
#define SIOCNT_CLK_2MHZ			(1 << 1)
#define SIOCNT_RECV_ENABLE		(1 << 2)
#define SIOCNT_SEND_ENABLE		(1 << 3)
#define SIOCNT_START			(1 << 7)
#define SIOCNT_XFER_LEN			(1 << 12)
#define SIOCNT_INT_ENABLE		(1 << 14)

/* only in 16bit multi player mode */
#define SIOCNT_BAUD_38400		1
#define SIOCNT_BAUD_57600		2
#define SIOCNT_BAUD_115200		3
#define SIOCNT_SI_STATUS		(1 << 2)
#define SIOCNT_SD_STATUS		(1 << 3)
#define SIOCNT_SLAVE1			(1 << 4)
#define SIOCNT_SLAVE2			(2 << 4)
#define SIOCNT_SLAVE3			(3 << 4)
#define SIOCNT_ERROR			(1 << 6)

/* REG_RCNT bits */
#define RCNT_GPIO_DATA_MASK		0x000f
#define RCNT_GPIO_DIR_MASK		0x00f0
#define RCNT_GPIO_INT_ENABLE	(1 << 8)
#define RCNT_MODE_GPIO			0x8000
#define RCNT_MODE_JOYBUS		0xc000


static void setup_sio(int bits, int ms);
static void setup_gpio(void);
static void comm_intr(void);

static int sio_bits, sio_master;
static void *sio_buf;


void comm_setup(int mode)
{
	int master = 0;

	mask(INTR_COMM);
	interrupt(INTR_COMM, comm_intr);
	unmask(INTR_COMM);

	switch(mode) {
	case COMM_SIO8_MASTER:
		master = 1;
	case COMM_SIO8_SLAVE:
		setup_sio(8, master);
		break;

	case COMM_SIO32_MASTER:
		master = 1;
	case COMM_SIO32_SLAVE:
		setup_sio(32, master);
		break;

	case COMM_GPIO:
		setup_gpio();
		break;

	default:
		panic("unimplemented comm mode\n");
	}
}

static void setup_sio(int bits, int ms)
{
	REG_RCNT = 0;	/* serial mode */

	sio_bits = bits;
	sio_master = ms;
}

void sio_transfer(void *in, const void *out)
{
	/* load outgoing data */
	if(sio_bits <= 8) {
		REG_SIODATA8 = *(unsigned char*)out;
	} else {
		REG_SIODATA32 = *(unsigned long*)out;
	}
	sio_buf = in;

	/* IE=1, external clock, send enable */
	REG_SIOCNT = SIOCNT_INT_ENABLE | SIOCNT_SEND_ENABLE;

	/* start transfer */
	REG_SIOCNT |= SIOCNT_START;

	/* wait until the transfer is complete */
	while(REG_SIOCNT & SIOCNT_START);
}

void sio_transfer_async(void *in, const void *out)
{
	/* load outgoing data */
	if(sio_bits <= 8) {
		REG_SIODATA8 = *(unsigned char*)out;
	} else {
		REG_SIODATA32 = *(unsigned long*)out;
	}
	sio_buf = in;

	/* IE=1, external clock, send enable */
	REG_SIOCNT = SIOCNT_INT_ENABLE | SIOCNT_SEND_ENABLE;

	/* start transfer */
	REG_SIOCNT |= SIOCNT_START;
}


static void setup_gpio(void)
{
	REG_RCNT = RCNT_MODE_GPIO | RCNT_GPIO_INT_ENABLE;
}

void gpio_dir(int dir_so, int dir_si, int dir_sd, int dir_sc)
{
	unsigned char mask;

	mask = ((dir_so & 1) << 7) | ((dir_si & 1) << 6) | ((dir_sd & 1) << 5) |
		((dir_sc & 1) << 4);
	gpio_dir_mask(mask);
}

void gpio_dir_mask(unsigned char dir)
{
	REG_RCNT = (REG_RCNT & 0xff0f) | ((dir & 0xf) << 4);
}

void gpio_set(unsigned char val)
{
	REG_RCNT = (REG_RCNT & 0xfff0) | (val & 0xf);
}

unsigned char gpio_get(void)
{
	return REG_RCNT & 0xf;
}

static void comm_intr(void)
{
	if(sio_buf) {
		if(REG_SIOCNT & SIOCNT_START) {
			panic("asio: interrupt with start bit == 1");
		}
		if(sio_bits <= 8) {
			*(unsigned char*)sio_buf = REG_SIODATA8;
		} else {
			*(unsigned long*)sio_buf = REG_SIODATA32;
		}
		sio_buf = 0;
	}

	if(signal_func(SIGIO)) {
		raise(SIGIO);
	}
}
