/*
 * timer.c
 *
 *  Created on: 2016年6月13日
 *      Author: yuan5
 */
#include <msp430.h>
#include "timer.h"
#include "misc.h"
#include "lcd.h"
#include "users.h"
#include "RFID.h"

//-----全局变量-----
unsigned char Timer_Counter = 0;				/*用于对按键按下时间进行计数*/

void Timer_init() {
	  TA0CCTL0 = CCIE;                          // TACCR0 interrupt enabled
	  TA0CCR0 = 65000;
	  TA0CTL = TASSEL__SMCLK | MC__CONTINOUS | ID__2;   // SMCLK, continuous mode
	  GPIO_init();
}

void GPIO_init()
{
	//-----设定P1.0和P9.7的输出初始值-----
	P1DIR |= BIT0;					//设定P1.0为输出
	P9DIR |= BIT7;
	P1OUT &= ~BIT0;					//设定P1.0初值
	P9OUT &= ~BIT7;					//设定P9.7初值
	//-----配合机械按键，启用内部上拉电阻-----
	P1REN |= BIT1;			        //启用P1.1 P1.2内部上下拉电阻
	P1DIR &= ~BIT1;
	P1OUT |= BIT1;		            //将电阻设置为上拉
	PM5CTL0 &= ~LOCKLPM5;           // Disable the GPIO power-on default high-impedance mode
									// to activate previously configured port settings
}

/******************************************************************************************************
 * 名       称：Key_SM()
 * 功       能：判断出长短键
 * 入口参数：无
 * 出口参数：无
 * 说       明：本状态机为Mealy型状态机，在Switch(State)中需要判断事件
 * 范       例：无
 ******************************************************************************************************/
void Key_SM()
{
	static unsigned char State;				//状态机的状态变量
	static unsigned char Key_Now;			//记录按键的当前电平
	unsigned char Key_Past=0;				//记录按键的前一次电平
	unsigned char Key_Dect=0;				//按键状态值
	Key_Past = Key_Now;
	//-----查询IO的输入寄存器-----
	if(P1IN&BIT1)
		Key_Now=1;
	else
		Key_Now=0;
	//-----电平前高后低，表明按下-----
	if((Key_Past==1)&&(Key_Now==0))
		Key_Dect=1;
	//-----电平前低后高，表明释放-----
	if((Key_Past==0)&&(Key_Now==1))
		Key_Dect=2 ;
	switch(State)			//该状态机靠扫描的按键值Key_Dect跳转状态
	{
	case IDLE:
		Timer_Counter=0;					//空闲状态对计数清零
		if(Key_Dect==1)
			State=SHORT;
			break;	//路径1
	case SHORT:
		if(Key_Dect==2)										   	//路径2
		{
			State=IDLE;
			P11_OnShortRelease();	//短按事件处理函数
		}
		if(LongClick_Dect())									//路径3
		{
			State=LONG;
			P11_OnLongClick();		//长按事件处理函数
		}
		break;
	case LONG:
		Timer_Counter=0; 					//长按状态对计数清零
		if(Key_Dect==2)
			State=IDLE;
		break;	 //路径4
	default:
		State=IDLE;
		break;
	}
}

 /******************************************************************************************************
  * 名       称：LongClick_Dect()
  * 功       能：对WDT中断计时，计满清零并返回”长键“信息
  * 入口参数：无
  * 出口参数：无
  * 说       明：
  * 范       例：无
  ******************************************************************************************************/
unsigned char LongClick_Dect()
{
	Timer_Counter++;
	if (Timer_Counter > COUNTER_THRESHOLD)
	{
		Timer_Counter=0;
		return(1);
	}
	else return(0);
}
/******************************************************************************************************
 * 名       称：P11_OnShortRelease()
 * 功       能：P1.1的短按事件处理函数，即当P1.1键被”短按“后，下一步干什么
 * 入口参数：无
 * 出口参数：无
 * 说       明：使用事件处理函数的形式，可以增强代码的移植性和可读性
 * 范       例：无
 ******************************************************************************************************/
void P11_OnShortRelease()					//P1.1的事件处理函数
{
	//----翻转P1.0IO电平-----
	//P1OUT ^= BIT0;
	if(M_State == M_WORK)
		return;
	++M_State;
	if(M_State > M_DEL_ALL)
		M_State = 1;
	switch (M_State) {
		case M_ADD:
			P9OUT |= BIT7;
			LCD_displayADD();
			break;
		case M_DELETE:
			LCD_displayDEL();
			P9OUT &= ~BIT7;
			break;
		case M_DEL_ALL:
			LCD_displayDELALL();
			break;
		default:
			break;
	}
	AddFlag = 0;
	DelFlag = 0;
	DelAll = 0;
	delay_ms(10);
}
/******************************************************************************************************
 * 名       称：P11_OnLongClick()
 * 功       能：P1.1的长按事件处理函数，即当P1.3键被”长按“后，下一步干什么
 * 入口参数：无
 * 出口参数：无
 * 说       明：使用事件处理函数的形式，可以增强代码的移植性和可读性
 * 范       例：无
 ******************************************************************************************************/
void P11_OnLongClick()					//P1.1的事件处理函数
{
	//----翻转P1.0IO电平-----
	//P1OUT ^= BIT0;
	if(M_State == M_WORK) {
		M_State = M_ADD;
		LCD_displayADD();
		P9OUT |= BIT7;
		P1OUT |= BIT0;
	}
	else {
		M_State = M_WORK;
		AddFlag = 0;
		DelFlag = 0;
		LCD_displayLine();
		P1OUT &= ~BIT0;
		P9OUT &= ~BIT7;
	}
	//LCD_displayDecimal(User_getUsersNum());
	delay_ms(10);
}

void Timer_setStartNum(unsigned int n) {

}

// Timer0_A0 interrupt service routine
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector = TIMER0_A0_VECTOR
__interrupt void Timer0_A0_ISR (void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(TIMER0_A0_VECTOR))) Timer0_A0_ISR (void)
#else
#error Compiler not supported!
#endif
{
	Key_SM();
	if(Gate_Flag) {
		Gate_OpenCount++;
		if(Gate_OpenCount > GATE_OPEN_THRESHOLD) {
			Gate_close();
			Gate_Flag = GATE_CLOSED;
			Gate_OpenCount = 0;
			LCD_displayLine();
		}
	}
	TA0CCR0 += 65000;                         // Add Offset to TA0CCR0
}
