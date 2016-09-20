/*
 * UART_init.c
 * 功能：初始化USCI_A0模块为UART模式，配置UART为波特率9600,8位数据,无校验,1位停止位
 *  Created on: 2013-3-18
 *  Author: Administrator
 */

#include <msp430.h>
#include "UART_Event.h"
#include "UART_Global.h"
/******************************************************************************************************
 * 名       称：USCI_A0_init()
 * 功       能：初始化USCI_A0模块为UART模式
 * 入口参数：无
 * 出口参数：无
 * 说       明：UART设为波特率9600,8位数据,无校验,1位停止位
 * 					UART初始化配置较复杂，可以使用Grace配置后再移植代码的方法
 * 范       例：无
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

//中断
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
        UCA0IFG &= ~UCRXIFG;          // 手动清除标志位
        UART_OnRx();							// 调用Rx事件处理函数
        break;
    case USCI_UART_UCTXIFG:
        UCA0IFG &= ~UCTXIFG;          // 手动清除标志位
        UART_OnTx();							// 调用Tx事件处理函数
        break;
    case USCI_UART_UCSTTIFG: break;
    case USCI_UART_UCTXCPTIFG: break;
    }
}
