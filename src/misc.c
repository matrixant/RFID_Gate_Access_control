/*
 * misc.c
 *
 *  Created on: 2016Äê6ÔÂ14ÈÕ
 *      Author: yuan5
 */
#include "msp430.h"
#include "misc.h"

unsigned char M_State = 0;
unsigned char Gate_OpenCount = 0;
unsigned char Gate_Flag = 0;

unsigned char Misc_dataMatch(unsigned char a[], unsigned char b[], unsigned char dataNum) {
	unsigned char i;
	for(i = 0; i < dataNum; ++i) {
		if(a[i] - b[i]) return 0;
	}
	return 1;
}
