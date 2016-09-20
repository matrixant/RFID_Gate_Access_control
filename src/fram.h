/*
 * fram.c
 *
 *  Created on: 2016Äê6ÔÂ13ÈÕ
 *      Author: yuan5
 */

#ifndef FRAM_C_
#define FRAM_C_

#define FRAM_DATA_START 0xF000

void FRAM_readBytes(unsigned char data[], unsigned char *address, unsigned char numOfBytes);
void FRAM_writeBytes(unsigned char data[], unsigned char *address, unsigned int numOfBytes);

#endif /* FRAM_C_ */
