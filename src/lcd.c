/*
 * lcd.c
 *
 *  Created on: 2016Äê6ÔÂ12ÈÕ
 *      Author: yuan5
 */
#include "driverlib.h"
#include "lcd.h"

unsigned char decimal[12] = {0xfc, 0x60, 0xdb, 0xf3, 0x67, 0xb7, 0xbf, 0xe4, 0xff, 0xf7, 0x03, 0x90};

void LCD_init() {
	__enable_interrupt();

    LCD_C_setPinAsLCDFunctionEx(LCD_C_BASE, LCD_C_SEGMENT_LINE_0,
                                LCD_C_SEGMENT_LINE_21);
    LCD_C_setPinAsLCDFunctionEx(LCD_C_BASE, LCD_C_SEGMENT_LINE_26,
                                LCD_C_SEGMENT_LINE_43);

    LCD_C_initParam initParams = {0};
    initParams.clockSource = LCD_C_CLOCKSOURCE_ACLK;
    initParams.clockDivider = LCD_C_CLOCKDIVIDER_1;
    initParams.clockPrescalar = LCD_C_CLOCKPRESCALAR_16;
    initParams.muxRate = LCD_C_4_MUX;
    initParams.waveforms = LCD_C_LOW_POWER_WAVEFORMS;
    initParams.segments = LCD_C_SEGMENTS_ENABLED;

    LCD_C_init(LCD_C_BASE, &initParams);

    // LCD Operation - VLCD generated internally, V2-V4 generated internally, v5 to ground
    LCD_C_setVLCDSource(LCD_C_BASE, LCD_C_VLCD_GENERATED_INTERNALLY,
                        LCD_C_V2V3V4_GENERATED_INTERNALLY_NOT_SWITCHED_TO_PINS,
                        LCD_C_V5_VSS);

    // Set VLCD voltage to 2.60v
    LCD_C_setVLCDVoltage(LCD_C_BASE,
                         LCD_C_CHARGEPUMP_VOLTAGE_2_60V_OR_2_17VREF);

    // Enable charge pump and select internal reference for it
    LCD_C_enableChargePump(LCD_C_BASE);
    LCD_C_selectChargePumpReference(LCD_C_BASE,
                                    LCD_C_INTERNAL_REFERENCE_VOLTAGE);

    LCD_C_configChargePump(LCD_C_BASE, LCD_C_SYNCHRONIZATION_ENABLED, 0);

    // Clear LCD memory
    LCD_C_clearMemory(LCD_C_BASE);
    //Turn LCD on
    LCD_C_on(LCD_C_BASE);
}

void LCD_displayPASS() {
	LCD_C_clearMemory(LCD_C_BASE);
	// LCD Pin6-Pin7 for 'P'
    LCD_C_setMemory(LCD_C_BASE, LCD_C_SEGMENT_LINE_6, 0xF);
    LCD_C_setMemory(LCD_C_BASE, LCD_C_SEGMENT_LINE_7, 0xC);

    // LCD Pin36-Pin37 for 'A'
    LCD_C_setMemory(LCD_C_BASE, LCD_C_SEGMENT_LINE_36, 0xF);
    LCD_C_setMemory(LCD_C_BASE, LCD_C_SEGMENT_LINE_37, 0xE);

    // LCD Pin28-Pin29 for 'S'
    LCD_C_setMemory(LCD_C_BASE, LCD_C_SEGMENT_LINE_28, 0x7);
    LCD_C_setMemory(LCD_C_BASE, LCD_C_SEGMENT_LINE_29, 0xB);

    // LCD Pin14-Pin15 for 'S'
    LCD_C_setMemory(LCD_C_BASE, LCD_C_SEGMENT_LINE_14, 0x7);
    LCD_C_setMemory(LCD_C_BASE, LCD_C_SEGMENT_LINE_15, 0xB);
}

void LCD_displayERROR() {
	LCD_C_clearMemory(LCD_C_BASE);
	// LCD Pin10-Pin11 for 'E'
    LCD_C_setMemory(LCD_C_BASE, LCD_C_SEGMENT_LINE_10, 0xF);
    LCD_C_setMemory(LCD_C_BASE, LCD_C_SEGMENT_LINE_11, 0x9);

    // LCD Pin6-Pin7 for 'R'
    LCD_C_setMemory(LCD_C_BASE, LCD_C_SEGMENT_LINE_6, 0xF);
    LCD_C_setMemory(LCD_C_BASE, LCD_C_SEGMENT_LINE_7, 0xE);

    // LCD Pin36-Pin37 for 'R'
    LCD_C_setMemory(LCD_C_BASE, LCD_C_SEGMENT_LINE_36, 0xF);
    LCD_C_setMemory(LCD_C_BASE, LCD_C_SEGMENT_LINE_37, 0xE);

    // LCD Pin28-Pin29 for 'O'
    LCD_C_setMemory(LCD_C_BASE, LCD_C_SEGMENT_LINE_28, 0xC);
    LCD_C_setMemory(LCD_C_BASE, LCD_C_SEGMENT_LINE_29, 0xF);

    // LCD Pin14-Pin15 for 'R'
    LCD_C_setMemory(LCD_C_BASE, LCD_C_SEGMENT_LINE_14, 0xF);
    LCD_C_setMemory(LCD_C_BASE, LCD_C_SEGMENT_LINE_15, 0xE);
}

void LCD_displayDEL() {
	LCD_C_clearMemory(LCD_C_BASE);
    // LCD Pin36-Pin37 for 'D'
    LCD_C_setMemory(LCD_C_BASE, LCD_C_SEGMENT_LINE_36, 0xC);
    LCD_C_setMemory(LCD_C_BASE, LCD_C_SEGMENT_LINE_37, 0xF);

    // LCD Pin28-Pin29 for 'E'
    LCD_C_setMemory(LCD_C_BASE, LCD_C_SEGMENT_LINE_28, 0xF);
    LCD_C_setMemory(LCD_C_BASE, LCD_C_SEGMENT_LINE_29, 0x9);

    // LCD Pin14-Pin15 for 'L'
    LCD_C_setMemory(LCD_C_BASE, LCD_C_SEGMENT_LINE_14, 0xC);
    LCD_C_setMemory(LCD_C_BASE, LCD_C_SEGMENT_LINE_15, 0x1);
}

void LCD_displayDELALL() {
	LCD_C_clearMemory(LCD_C_BASE);
	// LCD Pin18-Pin19 for 'D'
	LCD_C_setMemory(LCD_C_BASE, LCD_C_SEGMENT_LINE_18, 0xC);
	LCD_C_setMemory(LCD_C_BASE, LCD_C_SEGMENT_LINE_19, 0xF);

	// LCD Pin10-Pin11 for 'E'
    LCD_C_setMemory(LCD_C_BASE, LCD_C_SEGMENT_LINE_10, 0xF);
    LCD_C_setMemory(LCD_C_BASE, LCD_C_SEGMENT_LINE_11, 0x9);

    // LCD Pin6-Pin7 for 'L'
    LCD_C_setMemory(LCD_C_BASE, LCD_C_SEGMENT_LINE_6, 0xC);
    LCD_C_setMemory(LCD_C_BASE, LCD_C_SEGMENT_LINE_7, 0x1);

    // LCD Pin36-Pin37 for 'A'
    LCD_C_setMemory(LCD_C_BASE, LCD_C_SEGMENT_LINE_36, 0xF);
    LCD_C_setMemory(LCD_C_BASE, LCD_C_SEGMENT_LINE_37, 0xE);

    // LCD Pin28-Pin29 for 'L'
    LCD_C_setMemory(LCD_C_BASE, LCD_C_SEGMENT_LINE_28, 0xC);
    LCD_C_setMemory(LCD_C_BASE, LCD_C_SEGMENT_LINE_29, 0x1);

    // LCD Pin14-Pin15 for 'L'
    LCD_C_setMemory(LCD_C_BASE, LCD_C_SEGMENT_LINE_14, 0xC);
    LCD_C_setMemory(LCD_C_BASE, LCD_C_SEGMENT_LINE_15, 0x1);
}

void LCD_displayADD() {
	LCD_C_clearMemory(LCD_C_BASE);
    // LCD Pin36-Pin37 for 'A'
    LCD_C_setMemory(LCD_C_BASE, LCD_C_SEGMENT_LINE_36, 0xF);
    LCD_C_setMemory(LCD_C_BASE, LCD_C_SEGMENT_LINE_37, 0xE);

    // LCD Pin28-Pin29 for '0'
    LCD_C_setMemory(LCD_C_BASE, LCD_C_SEGMENT_LINE_28, 0xC);
    LCD_C_setMemory(LCD_C_BASE, LCD_C_SEGMENT_LINE_29, 0xF);

    // LCD Pin14-Pin15 for '0'
    LCD_C_setMemory(LCD_C_BASE, LCD_C_SEGMENT_LINE_14, 0xC);
    LCD_C_setMemory(LCD_C_BASE, LCD_C_SEGMENT_LINE_15, 0xF);
}

void LCD_displayLine() {
	unsigned char i;
	for (i = 0; i < 6; ++i) {
		LCD_setMemoryOnDigit(10, i);
	}
}

void LCD_displayParallelLine() {
	unsigned char i;
	for (i = 0; i < 6; ++i) {
		LCD_setMemoryOnDigit(11, i);
	}
}

void LCD_displayDecimal(unsigned long dec) {
	unsigned char decLen = LCD_decimalLen(dec);
	unsigned char i, tmp[6] = {0};
	for (i = 0; i < decLen; ++i) {
		tmp[i] = dec % 10;
		dec /= 10;
	}
	LCD_C_clearMemory(LCD_C_BASE);
	for (i = 0; i < decLen; ++i) {
		LCD_setMemoryOnDigit(tmp[i], i);
	}
}

unsigned char LCD_decimalLen(unsigned long dec) {
	if(dec > 99999) {
		return 6;
	}
	if(dec > 9999) {
		return 5;
	}
	if(dec > 999) {
		return 4;
	}
	if(dec > 99) {
		return 3;
	}
	if(dec > 9) {
		return 2;
	}
	return 1;
}

void LCD_setMemoryOnDigit(unsigned char dec, unsigned char digit) {
	unsigned char data = decimal[dec];
	switch(digit) {
	case 5:
		LCD_C_setMemory(LCD_C_BASE, LCD_C_SEGMENT_LINE_18, data % 16);
		LCD_C_setMemory(LCD_C_BASE, LCD_C_SEGMENT_LINE_19, data / 16);
		break;
	case 4:
		LCD_C_setMemory(LCD_C_BASE, LCD_C_SEGMENT_LINE_10, data % 16);
		LCD_C_setMemory(LCD_C_BASE, LCD_C_SEGMENT_LINE_11, data / 16);
		break;
	case 3:
		LCD_C_setMemory(LCD_C_BASE, LCD_C_SEGMENT_LINE_6, data % 16);
		LCD_C_setMemory(LCD_C_BASE, LCD_C_SEGMENT_LINE_7, data / 16);
		break;
	case 2:
		LCD_C_setMemory(LCD_C_BASE, LCD_C_SEGMENT_LINE_36, data % 16);
		LCD_C_setMemory(LCD_C_BASE, LCD_C_SEGMENT_LINE_37, data / 16);
		break;
	case 1:
		LCD_C_setMemory(LCD_C_BASE, LCD_C_SEGMENT_LINE_28, data % 16);
		LCD_C_setMemory(LCD_C_BASE, LCD_C_SEGMENT_LINE_29, data / 16);
		break;
	case 0:
		LCD_C_setMemory(LCD_C_BASE, LCD_C_SEGMENT_LINE_14, data % 16);
		LCD_C_setMemory(LCD_C_BASE, LCD_C_SEGMENT_LINE_15, data / 16);
		break;
	}
}
