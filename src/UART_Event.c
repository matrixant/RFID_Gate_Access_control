/*
 * UART_Event.c
 * 功能：全面处理与上位机通信的相关事宜
 *  Created on: 2013-3-18
 *      Author: Administrator
 */
#include <msp430.h>
#include "UARt_Global.h"
#include "UART_FIFO.h"
#include "UART_Event.h"
//-----对于硬件有关的代码宏定义处理-----
#define LED1_ON P1DIR|=BIT0; P1OUT|=BIT0
#define LED1_OFF P1DIR|=BIT0; P1OUT&=~BIT0

/******************************************************************************************************
 * 名       称：UART_OnTx()
 * 功       能：UART的Tx事件处理函数
 * 入口参数：无
 * 出口参数：无
 * 说       明：Tx_FIFO里有数据就将数据移到Tx Buffer寄存器中去
 * 范       例：无
 ******************************************************************************************************/
void UART_OnTx(void)
{
	if(Tx_FIFO_DataNum)
	{
		//Tx_FIFO_ReadChar(&temp);	//调用FIFO库函数
		UCA0TXBUF = Tx_FIFO_ReadChar();
	}
	while(UCA0IFG & UCTXIFG);
}
/******************************************************************************************************
 * 名       称：UART_OnRx()
 * 功       能：UART的Rx事件处理函数
 * 入口参数：无
 * 出口参数：无
 * 说       明：
 * 范       例：无
 ******************************************************************************************************/
void UART_OnRx(void)
{
	unsigned char temp = 0;
	temp = UCA0RXBUF;			// 预存下Tx Buffer数据
	//
	if (Rx_FIFO_DataNum < RX_FIFO_SIZE - 1) {
		Rx_FIFO_WriteChar(temp);
	}
}
/******************************************************************************************************
 * 名       称：UART_SendData()
 * 功       能：用UART发送一串数据
 * 入 口  参 数：*data：数据首地址
 * 出 口  参 数：无
 * 说       明：数据如果很长，超过Tx_FIFO长度，则会发生阻塞CPU
 * 范       例：无
 ******************************************************************************************************/
void UART_SendData(unsigned char addr[], unsigned char byteNum)
{
	unsigned char i;
	for (i = 0; i < byteNum; ++i) {
		Tx_FIFO_WriteChar(addr[i]);
	}
}
