/*
 * fram.c
 *
 *  Created on: 2016Äê6ÔÂ13ÈÕ
 *      Author: yuan5
 */
#include "driverlib.h"
#include "fram.h"

void FRAM_writeBytes(unsigned char data[], unsigned char *address, unsigned int numOfBytes) {
	FRAMCtl_write8(data, address, numOfBytes);
}

void FRAM_readBytes(unsigned char data[], unsigned char *address, unsigned char numOfBytes) {
	unsigned char i;
	for (i = 0; i < numOfBytes; ++i) {
		data[i] = *address++;
	}
}
