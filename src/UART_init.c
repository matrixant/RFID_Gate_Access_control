/*
 * UART_init.c
 * ���ܣ���ʼ��USCI_A0ģ��ΪUARTģʽ������UARTΪ������9600,8λ����,��У��,1λֹͣλ
 *  Created on: 2013-3-18
 *  Author: Administrator
 */

#include <msp430.h>
#include "UART_Event.h"
#include "UART_Global.h"
/******************************************************************************************************
 * ��       �ƣ�USCI_A0_init()
 * ��       �ܣ���ʼ��USCI_A0ģ��ΪUARTģʽ
 * ��ڲ�������
 * ���ڲ�������
 * ˵       ����UART��Ϊ������9600,8λ����,��У��,1λֹͣλ
 * 					UART��ʼ�����ýϸ��ӣ�����ʹ��Grace���ú�����ֲ����ķ���
 * ��       ������
 ******************************************************************************************************/
void USCI_A0_init(void)
{
	// Configure GPIO
	P2SEL0 |= BIT0 | BIT1;                    // USCI_A0 UART operation
	P2SEL1 &= ~(BIT0 | BIT1);

	// Disable the GPIO power-on default high-impedance mode to activate
	// previously configured port settings
	PM5CTL0 &= ~LOCKLPM5;

	// Configure USCI_A0 for UART mode
	UCA0CTLW0 = UCSWRST;                      // Put eUSCI in reset
	UCA0CTLW0 |= UCSSEL__SMCLK;               // CLK = SMCLK
	// Baud Rate calculation
	// 8000000/(16*9600) = 52.083
	// Fractional portion = 0.083
	// User's Guide Table 21-4: UCBRSx = 0x04
	// UCBRFx = int ( (52.083-52)*16) = 1
	UCA0BR0 = 26;                             // 8000000/16/9600
	UCA0BR1 = 0x00;
	UCA0MCTLW |= UCOS16 | UCBRF_1 | 0x4900;
	UCA0CTLW0 &= ~UCSWRST;                    // Initialize eUSCI
    UCA0IE |= UCRXIE | UCTXIE;                // Enable USCI_A0 RX Tx interrupt
    __enable_interrupt();
}

//�ж�
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=USCI_A0_VECTOR
__interrupt
#elif defined(__GNUC__)
__attribute__((interrupt(USCI_A0_VECTOR)))
#endif
void USCI_A0_ISR(void)
{
    switch(__even_in_range(UCA0IV,USCI_UART_UCTXCPTIFG))
    {
    case USCI_NONE: break;
    case USCI_UART_UCRXIFG:
        UCA0IFG &= ~UCRXIFG;          // �ֶ������־λ
        UART_OnRx();							// ����Rx�¼�������
        break;
    case USCI_UART_UCTXIFG:
        UCA0IFG &= ~UCTXIFG;          // �ֶ������־λ
        UART_OnTx();							// ����Tx�¼�������
        break;
    case USCI_UART_UCSTTIFG: break;
    case USCI_UART_UCTXCPTIFG: break;
    }
}
