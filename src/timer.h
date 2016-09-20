/*
 * timer.h
 *
 *  Created on: 2016年6月13日
 *      Author: yuan5
 */

#ifndef TIMER_H_
#define TIMER_H_

//-----对状态进行宏定义-----
#define IDLE				0
#define SHORT				1
#define LONG				2
#define COUNTER_THRESHOLD   100		/*长键判别门限*/

void Timer_init();
void Timer_setStartNum(unsigned int n);
void GPIO_init();
void Key_SM();
unsigned char LongClick_Dect();
void P11_OnShortRelease();
void P11_OnLongClick();

#endif /* TIMER_H_ */
