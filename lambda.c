#include <stdlib.h>

#include <r0ketlib/display.h>
#include <r0ketlib/print.h>
#include <r0ketlib/fonts.h>
#include <r0ketlib/keyin.h>
#include <r0ketlib/itoa.h>

#include "usetable.h"





#define SPEED_OF_LIGHT 299792458
// TODO: This in a vacuum, I need to divide this by 1.0003 or something like that



uint32_t cfreq = 10000; // Frequency in 0.01 mhz
uint8_t cstep = 4;



void display() {

	uint8_t i;
	uint32_t lambda;


	// Calculate digits
	uint8_t d_1 = ( cfreq / 100000 );
	uint8_t d_2 = (( cfreq / 10000 ) % 10);
	uint8_t d_3 = (( cfreq / 1000 ) % 10);
	uint8_t d_4 = (( cfreq / 100 ) % 10);
	uint8_t d_5 = (( cfreq / 10 ) % 10);
	uint8_t d_6 = (cfreq % 10);


	// Reset screen
	lcdFill(0xFF);
	lcdSetCrsr(0,0);


	// Display Frequency
	lcdPrint("Frequency:");
	lcdNl();
	lcdPrint(IntToStr(d_1,1,0));
	lcdPrint(IntToStr(d_2,1,0));
	lcdPrint(IntToStr(d_3,1,0));
	lcdPrint(IntToStr(d_4,1,0));
	lcdPrint(".");
	lcdPrint(IntToStr(d_5,1,0));
	lcdPrint(IntToStr(d_6,1,0));
	lcdPrint("MHz");
	lcdNl();


	// Display indicator
	if (cstep > 3) {
		lcdPrint(" ");
	}
	for (i=0; i<cstep; i=i+1) {
		lcdPrint(" ");
	}
	lcdPrint("^");
	lcdNl();
	lcdNl();



	// Calculate Lambda (In 0.1 mm)
	lambda = (SPEED_OF_LIGHT / cfreq);


	// Calculate Lambda digits
	d_1 = ( lambda / 100000 );
	d_2 = (( lambda / 10000 ) % 10);
	d_3 = (( lambda / 1000 ) % 10);
	d_4 = (( lambda / 100 ) % 10);
	d_5 = (( lambda / 10 ) % 10);
	d_6 = (lambda % 10);


	// Display Lambda
	lcdPrint("Lambda:");
	lcdNl();
	lcdPrint(IntToStr(d_1,1,0));
	lcdPrint(IntToStr(d_2,1,0));
	lcdPrint(IntToStr(d_3,1,0));
	lcdPrint(IntToStr(d_4,1,0));
	lcdPrint(".");
	lcdPrint(IntToStr(d_5,1,0));
	lcdPrint(IntToStr(d_6,1,0));
	lcdPrint("cm");
	lcdNl();



	// Render to the screen!
	lcdDisplay();

}



int power(int base, int exp) {
	int result = 1;
	while(exp) {
		result *= base;
		exp--;
	}
	return result;
}



void ram(void) {

	uint8_t key;

	while(1) {

		// Display current status
		display();

		// Wait for next key down
		key = getInputWait();
		// Wait for key release
		getInputWaitRelease();


		switch (key) {
			case BTN_UP:
				cfreq = cfreq + power(10, 5 - cstep);
				break;
			case BTN_DOWN:
				cfreq = cfreq - power(10, 5 - cstep);
				break;

			case BTN_RIGHT:
				if (cstep < 5) {
					cstep = cstep + 1;
				}
				break;
			case BTN_LEFT:
				if (cstep > 0) {
					cstep = cstep - 1;
				}
				break;
			case BTN_ENTER:
				return;
				break;
		}



	}

	return;

}
