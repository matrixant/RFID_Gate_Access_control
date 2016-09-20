/*
 * UART_Event.h
 *
 *  Created on: 2013-3-18
 *      Author: Administrator
 */

#ifndef UART_EVENT_H_
#define UART_EVENT_H_

extern void UART_OnTx(void);
extern void UART_OnRx(void);
extern void UART_SendData(unsigned char addr[], unsigned char byteNum);


#endif /* UART_EVENT_H_ */
