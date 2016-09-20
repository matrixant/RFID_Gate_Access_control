/*
 * RFID.h
 *
 *  Created on: 2016年6月8日
 *      Author: yuan5
 */

#ifndef RFID_H_
#define RFID_H_
#include "driverlib.h"
#include "UART_init.h"
#include "UART_Global.h"
#include "UART_Event.h"
#include "UART_FIFO.h"

//CY14443命令定义
#define   PCD_PN       0x01		  // 设备型号
#define   PCD_SN       0x02		  // 设备序列号
#define   PWR_DOWN     0x03		  //设备掉电，需重新加电复位

#define   FW_REL       0x10		  //固件版本号
#define   SOFTDOWN     0x11		  //模块软掉电
#define   HALT_CARD    0x12		  //中止卡操作
#define   AUTOSEARCH   0x13		  //自动寻卡
#define   BEEP         0x14		  //蜂鸣器开关
#define   BEEP_TIME    0x15		  //鸣响时间
#define   OUT1    	   0x16		  //OUT1输出
#define   OUT2    	   0x17		  //OUT2输出

#define   CARD_TYPE    0x19		  //读取卡类型
#define   CARD_SN      0x20		  //读卡序列号
#define   READ_BLOCK   0x21		  //读块数据
#define   WRITE_BLOCK  0x22		  //写块数据
#define   INIT_PURSE   0x23		  //初始化钱包
#define   READ_PURSE   0x24		  //读钱包
#define   ADD_PURSE    0x25		  //钱包充值
#define   DEC_PURSE    0x26		  //钱包扣款
#define   READ_SECTOR  0x2a       // 读扇区
#define   WRITE_SECTOR 0x2b       // 写扇区
#define   READ_E2      0x30       // 读E2
#define   WRITE_E2     0x31       // 写E2

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
