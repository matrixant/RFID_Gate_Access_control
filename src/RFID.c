/*
 * RFID.c
 *
 *  Created on: 2016年6月8日
 *      Author: yuan5
 */

#include "RFID.h"
#include "lcd.h"
#include "misc.h"
#include "users.h"

uint8_t RetData[RX_FIFO_SIZE] = {0}, AddFlag = 0, DelFlag = 0, DelAll = 0;
uint8_t Admin[] = {0x05, 0x01, 0x1D, 0x56, 0xC8};

void RFID_IO_init() {
    //P1.3 P1.2 中断口设置内部上拉电阻
    GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P1, GPIO_PIN2);
    GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P1, GPIO_PIN3);
    GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P1, GPIO_PIN5);

    //P1.3 P1.2 下降沿中断
    GPIO_selectInterruptEdge(GPIO_PORT_P1, GPIO_PIN2, GPIO_HIGH_TO_LOW_TRANSITION);
    GPIO_selectInterruptEdge(GPIO_PORT_P1, GPIO_PIN3, GPIO_HIGH_TO_LOW_TRANSITION);
    GPIO_selectInterruptEdge(GPIO_PORT_P1, GPIO_PIN5, GPIO_HIGH_TO_LOW_TRANSITION);

    //P4.2 电磁锁开关控制口
    GPIO_setAsOutputPin(GPIO_PORT_P4, GPIO_PIN2);
    GPIO_setOutputLowOnPin(GPIO_PORT_P4, GPIO_PIN2);

    //P4.3 电磁锁开关检测口
    //GPIO_setAsInputPinWithPullDownResistor(GPIO_PORT_P4, GPIO_PIN3);

    /*
     * Disable the GPIO power-on default high-impedance mode to activate
     * previously configured port settings
     */
    PMM_unlockLPM5();

    //P1.3 P1.2 清中断标志
    GPIO_clearInterrupt(GPIO_PORT_P1, GPIO_PIN3);
    GPIO_clearInterrupt(GPIO_PORT_P1, GPIO_PIN2);
    GPIO_clearInterrupt(GPIO_PORT_P1, GPIO_PIN5);

    //P1.3 P1.2 中断使能
    GPIO_enableInterrupt(GPIO_PORT_P1, GPIO_PIN3);
    GPIO_enableInterrupt(GPIO_PORT_P1, GPIO_PIN2);
    GPIO_enableInterrupt(GPIO_PORT_P1, GPIO_PIN5);
}

void RFID_init() {
	//初始化UART
	USCI_A0_init();
	//配置RFID寻卡中断引脚
	RFID_IO_init();
}

void RFID_sendCMD(uint8_t cmd[]) {
	cmd[cmd[0]] = RFID_checkSum(cmd);		//计算校验和
	Tx_FIFO_Clear();
	Rx_FIFO_Clear();
	uint8_t header[] = {0xaa, 0xbb};
	UART_SendData(header, 2);
	UART_SendData(cmd, cmd[0] + 1);

}

uint8_t RFID_checkSum(uint8_t dataBuf[]) {
	unsigned char tmp = dataBuf[0]-1;
	unsigned char chksum = dataBuf[0];

	while(tmp) {
		chksum^=dataBuf[tmp];
		tmp--;
	}
	return chksum;
}

void RFID_setAutoSearch(uint8_t flag) {		//0x13
	uint8_t cmd[] = { 0x03, AUTOSEARCH, flag, 0x00 };
	RFID_sendCMD(cmd);
}

void RFID_enterPowerSaving() {
	uint8_t cmd[] = { 0x02, HALT_CARD, 0x00 };
	RFID_sendCMD(cmd);
}

void RFID_openBuzzer() {
	uint8_t cmd[] = { 0x03, BEEP, 0x11, 0x00 };
	RFID_sendCMD(cmd);
}

void RFID_closeBuzzer() {
	uint8_t cmd[] = { 0x03, BEEP, 0x00, 0x00 };
	RFID_sendCMD(cmd);
}

void RFID_setBuzzerInterval(uint8_t ms) {
	uint8_t cmd[] = { 0x03, BEEP_TIME, 0x11, 0x00 };
	RFID_sendCMD(cmd);
}

void RFID_getModuleCode(uint8_t moduleCode[]) {
	uint8_t cmd[] = { 0x02, PCD_SN, 0x00 };
	RFID_sendCMD(cmd);
	delay_ms(1);
	RFID_pickData(moduleCode);
}

void RFID_getCardType(uint8_t cardType[]) {
	uint8_t cmd[] = { 0x02, CARD_TYPE, 0x00 };
	RFID_sendCMD(cmd);
	delay_ms(1);
	RFID_pickData(cardType);
}

void RFID_getCardID(uint8_t idNumber[]) {
	uint8_t cmd[] = { 0x02, CARD_SN, 0x22 };
	RFID_sendCMD(cmd);
	RFID_pickData(idNumber);
	UART_SendData(idNumber, idNumber[0]);
	switch (M_State) {
		case M_WORK:
			if(User_getUserIndex(&idNumber[1]) > 0) {
				Gate_open();
				Gate_Flag = GATE_OPENED;
				LCD_displayPASS();
				break;
			}
			else
				LCD_displayERROR();
			break;
		case M_ADD:
			if(AddFlag) {
				LCD_displayDecimal(User_addUser(&idNumber[1]));
			}
			else {
				if(Misc_dataMatch(idNumber, Admin, 5)) {
					AddFlag = 1;
					LCD_displayPASS();
				}
			}
			break;
		case M_DELETE:
			if(DelFlag) {
				LCD_displayDecimal(User_deleteUserOnIndex(User_getUserIndex(&idNumber[1])));
			}
			else {
				if(Misc_dataMatch(idNumber, Admin, 5)) {
					DelFlag = 1;
					LCD_displayPASS();
				}
			}
			break;
		case M_DEL_ALL:
			if(DelAll >= 2) {
				User_deleteAllUsers();
				LCD_displayPASS();
			}
			else {
				if(Misc_dataMatch(idNumber, Admin, 5)) {
					DelAll++;
					LCD_displayParallelLine();
				}
			}
			break;
		default:
			break;
	}
}

void RFID_pickData(uint8_t data[]) {
	//UCA0IE &= ~(UCRXIE | UCTXIE);                         // Disable USCI_A0 RX Tx interrupt
	while(UCA0IFG & UCTXIFG);
	delay_ms(10);
	UCA0IE &= ~UCRXIE;                         // Disable USCI_A0 RX Tx interrupt
	uint8_t i;
	data[0] = Rx_FIFO[2] - 1;
	for (i = 1; i < data[0]; ++i) {
		data[i] = Rx_FIFO[i + 3];
	}
	UCA0IE |= UCRXIE;                         //Enable USCI_A0 RX Tx interrupt
}



//******************************************************************************
//
//This is the PORT1_VECTOR interrupt vector service routine
//
//******************************************************************************
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=PORT1_VECTOR
__interrupt
#elif defined(__GNUC__)
__attribute__((interrupt(PORT1_VECTOR)))
#endif
void Port_1(void)
{
	if(GPIO_getInterruptStatus(GPIO_PORT_P1, GPIO_PIN3)) {
		//P1.3 IFG cleared
		GPIO_clearInterrupt(GPIO_PORT_P1, GPIO_PIN3);
//		if(!(P4IN & BIT3)) {
		if(!Gate_Flag) {
			RFID_getCardID(RetData);
//			delay_ms(900);
		}
	}
	if(GPIO_getInterruptStatus(GPIO_PORT_P1, GPIO_PIN2)) {
		GPIO_clearInterrupt(GPIO_PORT_P1, GPIO_PIN2);
		if(!Gate_Flag && M_State == M_WORK) {
			Gate_open();
			Gate_Flag = GATE_OPENED;
		}
	}
    if(GPIO_getInterruptStatus(GPIO_PORT_P1, GPIO_PIN5)) {
		GPIO_clearInterrupt(GPIO_PORT_P1, GPIO_PIN5);
		if(!Gate_Flag && M_State == M_WORK) {
			Gate_open();
			Gate_Flag = GATE_OPENED;
		}
	}
    __bic_SR_register_on_exit(LPM0_bits);
}
