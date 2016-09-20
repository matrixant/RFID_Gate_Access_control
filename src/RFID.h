/*
 * RFID.h
 *
 *  Created on: 2016��6��8��
 *      Author: yuan5
 */

#ifndef RFID_H_
#define RFID_H_
#include "driverlib.h"
#include "UART_init.h"
#include "UART_Global.h"
#include "UART_Event.h"
#include "UART_FIFO.h"

//CY14443�����
#define   PCD_PN       0x01		  // �豸�ͺ�
#define   PCD_SN       0x02		  // �豸���к�
#define   PWR_DOWN     0x03		  //�豸���磬�����¼ӵ縴λ

#define   FW_REL       0x10		  //�̼��汾��
#define   SOFTDOWN     0x11		  //ģ�������
#define   HALT_CARD    0x12		  //��ֹ������
#define   AUTOSEARCH   0x13		  //�Զ�Ѱ��
#define   BEEP         0x14		  //����������
#define   BEEP_TIME    0x15		  //����ʱ��
#define   OUT1    	   0x16		  //OUT1���
#define   OUT2    	   0x17		  //OUT2���

#define   CARD_TYPE    0x19		  //��ȡ������
#define   CARD_SN      0x20		  //�������к�
#define   READ_BLOCK   0x21		  //��������
#define   WRITE_BLOCK  0x22		  //д������
#define   INIT_PURSE   0x23		  //��ʼ��Ǯ��
#define   READ_PURSE   0x24		  //��Ǯ��
#define   ADD_PURSE    0x25		  //Ǯ����ֵ
#define   DEC_PURSE    0x26		  //Ǯ���ۿ�
#define   READ_SECTOR  0x2a       // ������
#define   WRITE_SECTOR 0x2b       // д����
#define   READ_E2      0x30       // ��E2
#define   WRITE_E2     0x31       // дE2

#define true 1
#define false 0

extern uint8_t AddFlag;
extern uint8_t DelFlag;
extern uint8_t DelAll;

void RFID_init();
void RFID_sendCMD(uint8_t cmd[]);
uint8_t RFID_checkSum(uint8_t dataBuf[]);
void RFID_setAutoSearch(uint8_t flag);			//0x13
void RFID_enterPowerSaving();					//0x12
void RFID_openBuzzer();							//0x14
void RFID_closeBuzzer();						//0x14
void RFID_setBuzzerInterval(uint8_t ms);		//0x15
void RFID_getModuleCode(uint8_t moduleCode[]);	//0x02
void RFID_getCardType(uint8_t cardType[]);		//0x19
void RFID_getCardID(uint8_t idNumber[]);		//0x20
void RFID_pickData(uint8_t data[]);

#endif /* RFID_H_ */
