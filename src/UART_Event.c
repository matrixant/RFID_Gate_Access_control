/*
 * UART_Event.c
 * ���ܣ�ȫ�洦������λ��ͨ�ŵ��������
 *  Created on: 2013-3-18
 *      Author: Administrator
 */
#include <msp430.h>
#include "UARt_Global.h"
#include "UART_FIFO.h"
#include "UART_Event.h"
//-----����Ӳ���йصĴ���궨�崦��-----
#define LED1_ON P1DIR|=BIT0; P1OUT|=BIT0
#define LED1_OFF P1DIR|=BIT0; P1OUT&=~BIT0

/******************************************************************************************************
 * ��       �ƣ�UART_OnTx()
 * ��       �ܣ�UART��Tx�¼�������
 * ��ڲ�������
 * ���ڲ�������
 * ˵       ����Tx_FIFO�������ݾͽ������Ƶ�Tx Buffer�Ĵ�����ȥ
 * ��       ������
 ******************************************************************************************************/
void UART_OnTx(void)
{
	if(Tx_FIFO_DataNum)
	{
		//Tx_FIFO_ReadChar(&temp);	//����FIFO�⺯��
		UCA0TXBUF = Tx_FIFO_ReadChar();
	}
	while(UCA0IFG & UCTXIFG);
}
/******************************************************************************************************
 * ��       �ƣ�UART_OnRx()
 * ��       �ܣ�UART��Rx�¼�������
 * ��ڲ�������
 * ���ڲ�������
 * ˵       ����
 * ��       ������
 ******************************************************************************************************/
void UART_OnRx(void)
{
	unsigned char temp = 0;
	temp = UCA0RXBUF;			// Ԥ����Tx Buffer����
	//
	if (Rx_FIFO_DataNum < RX_FIFO_SIZE - 1) {
		Rx_FIFO_WriteChar(temp);
	}
}
/******************************************************************************************************
 * ��       �ƣ�UART_SendData()
 * ��       �ܣ���UART����һ������
 * �� ��  �� ����*data�������׵�ַ
 * �� ��  �� ������
 * ˵       ������������ܳ�������Tx_FIFO���ȣ���ᷢ������CPU
 * ��       ������
 ******************************************************************************************************/
void UART_SendData(unsigned char addr[], unsigned char byteNum)
{
	unsigned char i;
	for (i = 0; i < byteNum; ++i) {
		Tx_FIFO_WriteChar(addr[i]);
	}
}
