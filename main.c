#include "RFID.h"
#include "lcd.h"
#include "timer.h"
#include "fram.h"
#include "users.h"
#include "misc.h"

void main(void) {
	void initBoard();

    WDT_A_hold(WDT_A_BASE);
    initBoard();
    delay_ms(100);
    
    LCD_init();
    Timer_init();
    RFID_init();
    User_init();
    
    delay_ms(1000);
    LCD_displayLine();

    __bis_SR_register(LPM0_bits | GIE);
    //while(1);
}

void initBoard() {
	  // Startup clock system with max DCO setting ~8MHz
	  CSCTL0_H = CSKEY >> 8;                    // Unlock clock registers
	  CSCTL1 = DCOFSEL_3 | DCORSEL;             // Set DCO to 8MHz
	  CSCTL2 = SELA__VLOCLK | SELS__DCOCLK | SELM__DCOCLK;
	  CSCTL3 = DIVA__1 | DIVS__1 | DIVM__1;     // Set all dividers
	  CSCTL0_H = 0;                             // Lock CS registers
}
