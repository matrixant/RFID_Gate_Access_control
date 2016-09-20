/*
 * misc.h
 *
 *  Created on: 2016年6月14日
 *      Author: yuan5
 */

#ifndef MISC_H_
#define MISC_H_

#define Gate_open()			P4OUT |= BIT2
#define Gate_close()		P4OUT &= ~BIT2
#define GATE_OPENED			1
#define GATE_CLOSED			0
#define M_WORK				0
#define M_ADD				1
#define M_DELETE			2
#define M_DEL_ALL			3
#define GATE_OPEN_THRESHOLD 60

#define CPU_F ((double)8000000)		//8000000为时钟，可以根据自己时钟设置。
#define delay_us(x) __delay_cycles((long)(CPU_F*(double)x/1000000.0))
#define delay_ms(x) __delay_cycles((long)(CPU_F*(double)x/1000.0))

extern unsigned char M_State;
extern unsigned char Gate_OpenCount;
extern unsigned char Gate_Flag;

unsigned char Misc_dataMatch(unsigned char a[], unsigned char b[], unsigned char dataNum);

#endif /* MISC_H_ */
