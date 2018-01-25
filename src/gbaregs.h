/*
gbasys - a gameboy advance hardware abstraction library
Copyright (C) 2004-2018  John Tsiombikas <nuclear@member.fsf.org>

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
#ifndef GBAREGS_H_
#define GBAREGS_H_

#define REG_BASE		0x4000000
#define REG16(x)		(*(volatile uint16_t*)(REG_BASE + (x)))
#define REG32(x)		(*(volatile uint32_t*)(REG_BASE + (x)))

/* ---- display registers ---- */
#define REG_DISPCNT		REG16(0x00)
#define REG_GREENSWAP	REG16(0x02)
#define REG_DISPSTAT	REG16(0x04)
#define REG_VCOUNT		REG16(0x06)
#define REG_BG0CNT		REG16(0x08)
#define REG_BG1CNT		REG16(0x0a)
#define REG_BG2CNT		REG16(0x0c)
#define REG_BG3CNT		REG16(0x0e)
/* scrolling registers */
#define REG_BG0HOFS		REG16(0x10)
#define REG_BG0VOFS		REG16(0x12)
#define REG_BG1HOFS		REG16(0x14)
#define REG_BG1VOFS		REG16(0x16)
#define REG_BG2HOFS		REG16(0x18)
#define REG_BG2VOFS		REG16(0x1a)
#define REG_BG3HOFS		REG16(0x1c)
#define REG_BG3VOFS		REG16(0x1e)
/* BG rotation and scaling registers */
#define REG_BG2PA		REG16(0x20)
#define REG_BG2PB		REG16(0x22)
#define REG_BG2PC		REG16(0x24)
#define REG_BG2PD		REG16(0x26)
#define REG_BG2X		REG32(0x28)
#define REG_BG2Y		REG32(0x2c)
#define REG_BG3PA		REG16(0x30)
#define REG_BG3PB		REG16(0x32)
#define REG_BG3PC		REG16(0x34)
#define REG_BG3PD		REG16(0x36)
#define REG_BG3X		REG32(0x38)
#define REG_BG3Y		REG32(0x3c)
/* window registers */
#define REG_WIN0H		REG16(0x40)
#define REG_WIN1H		REG16(0x42)
#define REG_WIN0V		REG16(0x44)
#define REG_WIN1V		REG16(0x46)
#define REG_WININ		REG16(0x48)
#define REG_WINOUT		REG16(0x4a)

#define REG_MOSAIC		REG16(0x4c)
/* color effects */
#define REG_BLDCNT		REG16(0x50)
#define REG_BLDALPHA	REG16(0x52)
#define REG_BLDY		REG16(0x54)

/* ---- sound registers ---- */
#define REG_SOUND1CNT_L	REG16(0x60)
#define REG_SOUND1CNT_H	REG16(0x62)
#define REG_SOUND1CNT_X	REG16(0x64)
#define REG_SOUND2CNT_L	REG16(0x68)
#define REG_SOUND2CNT_H	REG16(0x6c)
#define REG_SOUND3CNT_L	REG16(0x70)
#define REG_SOUND3CNT_H	REG16(0x72)
#define REG_SOUND3CNT_X	REG16(0x74)
#define REG_SOUND4CNT_L	REG16(0x78)
#define REG_SOUND4CNT_H	REG16(0x7c)
#define REG_SOUNDCNT_L	REG16(0x80)
#define REG_SOUNDCNT_H	REG16(0x82)
#define REG_SOUNDCNT_X	REG16(0x84)
#define REG_SOUNDBIAS	REG16(0x88)
#define WAVE_RAM_PTR	((unsigned char*)(REG_BASE + 0x90))
#define REG_FIFO_A		REG32(0xa0)
#define REG_FIFO_B		REG32(0xa4)

/* ---- DMA registers ---- */
#define REG_DMA0SAD		REG32(0xb0)
#define REG_DMA0DAD		REG32(0xb4)
#define REG_DMA0CNT_L	REG16(0xb8)
#define REG_DMA0CNT_H	REG16(0xba)
#define REG_DMA1SAD		REG32(0xbc)
#define REG_DMA1DAD		REG32(0xc0)
#define REG_DMA1CNT_L	REG16(0xc4)
#define REG_DMA1CNT_H	REG16(0xc6)
#define REG_DMA2SAD		REG32(0xc8)
#define REG_DMA2DAD		REG32(0xcc)
#define REG_DMA2CNT_L	REG16(0xd0)
#define REG_DMA2CNT_H	REG16(0xd2)
#define REG_DMA3SAD		REG32(0xd4)
#define REG_DMA3DAD		REG32(0xd8)
#define REG_DMA3CNT_L	REG16(0xdc)
#define REG_DMA3CNT_H	REG16(0xde)

/* ---- timer registers ---- */
#define REG_TM0CNT_L	REG16(0x100)
#define REG_TM0CNT_H	REG16(0x102)
#define REG_TM1CNT_L	REG16(0x104)
#define REG_TM1CNT_H	REG16(0x106)
#define REG_TM2CNT_L	REG16(0x108)
#define REG_TM2CNT_H	REG16(0x10a)
#define REG_TM3CNT_L	REG16(0x10c)
#define REG_TM3CNT_H	REG16(0x10e)

/* ---- communication registers (serial/joybus/gpio) ---- */
#define REG_SIODATA32	REG32(0x120)
#define REG_SIOMULTI0	REG16(0x120)
#define REG_SIOMULTI1	REG16(0x122)
#define REG_SIOMULTI2	REG16(0x124)
#define REG_SIOMULTI3	REG16(0x126)
#define REG_SIOCNT		REG16(0x128)
#define REG_SIOMLT_SEND	REG16(0x12a)
#define REG_SIODATA8	REG16(0x12a)
#define REG_RCNT		REG16(0x134)
#define REG_JOYCNT		REG16(0x140)
#define REG_JOY_RECV	REG32(0x150)
#define REG_JOY_TRANS	REG32(0x154)
#define REG_JOYSTAT		REG16(0x158)

/* ---- keypad registers ---- */
#define REG_KEYINPUT	REG16(0x130)
#define REG_KEYCNT		REG16(0x132)

/* ---- interrupts ---- */
#define REG_IE			REG16(0x200)
#define REG_IF			REG16(0x202)
#define REG_WAITCNT		REG16(0x204)
#define REG_IME			REG16(0x208)

#define REG_POSTFLG		REG8(0x300)
#define REG_HALTCNT		REG8(0x301)
#define REG_INTMEMCNT	REG32(0x800)

#endif	/* GBAREGS_H_ */
