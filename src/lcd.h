/*
 * lcd.h
 *
 *  Created on: 2016Äê6ÔÂ12ÈÕ
 *      Author: yuan5
 */

#ifndef LCD_H_
#define LCD_H_

void LCD_init();
void LCD_displayPASS();
void LCD_displayERROR();
void LCD_displayADD();
void LCD_displayDEL();
void LCD_displayDELALL();
void LCD_displayLine();
void LCD_displayParallelLine();
void LCD_displayDecimal(unsigned long dec);
unsigned char LCD_decimalLen(unsigned long dec);
void LCD_setMemoryOnDigit(unsigned char dec, unsigned char digit);

#endif /* LCD_H_ */
