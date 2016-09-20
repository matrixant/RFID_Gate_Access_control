/*
 * timer.h
 *
 *  Created on: 2016��6��13��
 *      Author: yuan5
 */

#ifndef TIMER_H_
#define TIMER_H_

//-----��״̬���к궨��-----
#define IDLE				0
#define SHORT				1
#define LONG				2
#define COUNTER_THRESHOLD   100		/*�����б�����*/

void Timer_init();
void Timer_setStartNum(unsigned int n);
void GPIO_init();
void Key_SM();
unsigned char LongClick_Dect();
void P11_OnShortRelease();
void P11_OnLongClick();

#endif /* TIMER_H_ */
