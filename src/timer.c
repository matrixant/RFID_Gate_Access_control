/*
 * timer.c
 *
 *  Created on: 2016��6��13��
 *      Author: yuan5
 */
#include <msp430.h>
#include "timer.h"
#include "misc.h"
#include "lcd.h"
#include "users.h"
#include "RFID.h"

//-----ȫ�ֱ���-----
unsigned char Timer_Counter = 0;				/*���ڶ԰�������ʱ����м���*/

void Timer_init() {
	  TA0CCTL0 = CCIE;                          // TACCR0 interrupt enabled
	  TA0CCR0 = 65000;
	  TA0CTL = TASSEL__SMCLK | MC__CONTINOUS | ID__2;   // SMCLK, continuous mode
	  GPIO_init();
}

void GPIO_init()
{
	//-----�趨P1.0��P9.7�������ʼֵ-----
	P1DIR |= BIT0;					//�趨P1.0Ϊ���
	P9DIR |= BIT7;
	P1OUT &= ~BIT0;					//�趨P1.0��ֵ
	P9OUT &= ~BIT7;					//�趨P9.7��ֵ
	//-----��ϻ�е�����������ڲ���������-----
	P1REN |= BIT1;			        //����P1.1 P1.2�ڲ�����������
	P1DIR &= ~BIT1;
	P1OUT |= BIT1;		            //����������Ϊ����
	PM5CTL0 &= ~LOCKLPM5;           // Disable the GPIO power-on default high-impedance mode
									// to activate previously configured port settings
}

/******************************************************************************************************
 * ��       �ƣ�Key_SM()
 * ��       �ܣ��жϳ����̼�
 * ��ڲ�������
 * ���ڲ�������
 * ˵       ������״̬��ΪMealy��״̬������Switch(State)����Ҫ�ж��¼�
 * ��       ������
 ******************************************************************************************************/
void Key_SM()
{
	static unsigned char State;				//״̬����״̬����
	static unsigned char Key_Now;			//��¼�����ĵ�ǰ��ƽ
	unsigned char Key_Past=0;				//��¼������ǰһ�ε�ƽ
	unsigned char Key_Dect=0;				//����״ֵ̬
	Key_Past = Key_Now;
	//-----��ѯIO������Ĵ���-----
	if(P1IN&BIT1)
		Key_Now=1;
	else
		Key_Now=0;
	//-----��ƽǰ�ߺ�ͣ���������-----
	if((Key_Past==1)&&(Key_Now==0))
		Key_Dect=1;
	//-----��ƽǰ�ͺ�ߣ������ͷ�-----
	if((Key_Past==0)&&(Key_Now==1))
		Key_Dect=2 ;
	switch(State)			//��״̬����ɨ��İ���ֵKey_Dect��ת״̬
	{
	case IDLE:
		Timer_Counter=0;					//����״̬�Լ�������
		if(Key_Dect==1)
			State=SHORT;
			break;	//·��1
	case SHORT:
		if(Key_Dect==2)										   	//·��2
		{
			State=IDLE;
			P11_OnShortRelease();	//�̰��¼�������
		}
		if(LongClick_Dect())									//·��3
		{
			State=LONG;
			P11_OnLongClick();		//�����¼�������
		}
		break;
	case LONG:
		Timer_Counter=0; 					//����״̬�Լ�������
		if(Key_Dect==2)
			State=IDLE;
		break;	 //·��4
	default:
		State=IDLE;
		break;
	}
}

 /******************************************************************************************************
  * ��       �ƣ�LongClick_Dect()
  * ��       �ܣ���WDT�жϼ�ʱ���������㲢���ء���������Ϣ
  * ��ڲ�������
  * ���ڲ�������
  * ˵       ����
  * ��       ������
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
 * ��       �ƣ�P11_OnShortRelease()
 * ��       �ܣ�P1.1�Ķ̰��¼�������������P1.1�������̰�������һ����ʲô
 * ��ڲ�������
 * ���ڲ�������
 * ˵       ����ʹ���¼�����������ʽ��������ǿ�������ֲ�ԺͿɶ���
 * ��       ������
 ******************************************************************************************************/
void P11_OnShortRelease()					//P1.1���¼�������
{
	//----��תP1.0IO��ƽ-----
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
 * ��       �ƣ�P11_OnLongClick()
 * ��       �ܣ�P1.1�ĳ����¼�������������P1.3����������������һ����ʲô
 * ��ڲ�������
 * ���ڲ�������
 * ˵       ����ʹ���¼�����������ʽ��������ǿ�������ֲ�ԺͿɶ���
 * ��       ������
 ******************************************************************************************************/
void P11_OnLongClick()					//P1.1���¼�������
{
	//----��תP1.0IO��ƽ-----
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
