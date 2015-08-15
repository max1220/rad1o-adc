#include <libopencm3/lpc43xx/gpio.h>
#include <libopencm3/lpc43xx/scu.h>
#include <libopencm3/lpc43xx/dac.h>
#include <libopencm3/lpc43xx/adc.h>

#include <rad1olib/setup.h>
#include <rad1olib/pins.h>

#include <r0ketlib/display.h>
#include <r0ketlib/itoa.h>
#include <r0ketlib/print.h>
#include <r0ketlib/keyin.h>
#include <r0ketlib/idle.h>

#include <fatfs/ff.h>
#include <r0ketlib/fs_util.h>
#include <r0ketlib/select.h>

#include "usetable.h"

#define RGB1(r,g,b) (((r)&0b11111000)|((g)>>5))
#define RGB2(r,g,b) (((g)&0b00011100)<<3|((b)>>3))
#define RGB(r,g,b) ((RGB1(r,g,b)<<8) | RGB2(r,g,b))

int channels[13] = {
	ADC_CR_CH1, 1,
	ADC_CR_CH2, 1,
	ADC_CR_CH3, 8,
	ADC_CR_CH4, 1,
	ADC_CR_CH5, 8,
	ADC_CR_CH6, 8,
	ADC_CR_CH7, 4
};

int channel = 5;

uint8_t values[130] = {0xDD};

void addValue(uint8_t val) {
	int i;
	for (i=1; i < 131; i++) {
			values[i-1] = values[i];
	}
	values[130] = val;
}

void drawValues() {
	int x,y;
	lcd_select();

	for (x=0; x < 130; x++) {
		for (y=0; y < 130; y++) {
			// Background
			lcdSetPixel(x, y, RGB(0x00,0x00,0x00));
		}
	}
	for (x=0; x < 130; x++) {
		// Cross
		lcdSetPixel(64, x, RGB(0xFF,0x00,0xFF));
		lcdSetPixel(x, 64, RGB(0xFF,0x00,0xFF));
	}
	for (x=0; x < 130; x++) {
		// Value
		lcdSetPixel(x, values[x] / channels[(channel*2) + 1], RGB(0xFF,0xFF,0xFF));
	}

	lcd_deselect();

	lcdSetCrsr(0,0);
	lcdPrint("C: ");
	lcdPrintln(IntToStr(channel, 1, F_HEX));
	lcdPrint("S: ");
	lcdPrintln(IntToStr(channels[(channel*2) + 1], 1, F_LONG));
	lcdPrint("V: ");
	lcdPrintln(IntToStr(values[130] + 1, 4, F_LONG));

}


void ram(void) {
	int i;
	uint8_t adc_val;

	getInputWaitRelease();
	lcdClear(0x00);

	SETUPgout(MIC_AMP_DIS);
	OFF(MIC_AMP_DIS); // Enable AMP
	dac_init(false);

	while(1) {
		while (getInputRaw()==BTN_NONE) {
			addValue(adc_get_single(ADC0,channels[channel * 2]));
			drawValues();
			lcdDisplay();
		}
		if(getInputRaw() == BTN_ENTER) {
			return;
		}
		if(getInputRaw() == BTN_LEFT) {
			channel += 1;
			channel = channel % 7;
			delayms(500);
		}
	}
}
