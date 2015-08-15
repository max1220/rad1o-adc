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
#include <r0ketlib/fs_util.h>
#include <r0ketlib/select.h>
#include <r0ketlib/idle.h>

#include <fatfs/ff.h>

#include <math.h>

#include "usetable.h"

#define RGB1(r,g,b) (((r)&0b11111000)|((g)>>5))
#define RGB2(r,g,b) (((g)&0b00011100)<<3|((b)>>3))
#define RGB(r,g,b) ((RGB1(r,g,b)<<8) | RGB2(r,g,b))

#define PI 3.141592653589793238462643
#define NFFT 256

int data[NFFT * 2] = {0};

void fft(short *x) {
  static short once=1, sint[3 * NFFT / 4];
  int n1, n2, ie, i, j, k, xt, yt;
  short *xi, *xl, *sinus;

  if (once) {
    for (i=0; i < 3*NFFT/4; i++) {
      sint[i] = 0x7FFF * sin(2 * PI * i / NFFT);
    }
    once = 0;
  }

  for (n2=NFFT, ie=1; ie<NFFT; ie=ie<<1) {
    n1 = n2;
    n2 = n2 >> 1;
    sinus = sint;
    for (j=0; j < n2; j++) {
      for (i=j<<1; i < 2*NFFT; i+=n1<<1) {
        xi = x + i;
        xl = xi + n1;
        xt = (xi[0] - xl[0]) >> 1;
        yt = (xi[1] - xl[1]) >> 1;
        *(int*)xi = (((xi[0] + xl[0]) << 15) & 0xFFFF0000) | (unsigned short)((xi[1] + xl[1]) >> 1);
        *(int*)xl = (((xt * sinus[NFFT/4] + yt * sinus[0]) << 1) & 0xFFFF0000) | (unsigned short)((yt * sinus[NFFT/4] - xt * sinus[0]) >> 15);
      }
      sinus = sinus + ie;
    }
  }

  for (j=i=0; i < 2*NFFT; i+=2) {
    if (i < j) {
      ie = *(int*)(x+i);
      *(int*)(x+i) = *(int*)(x+j);
      *(int*)(x+j) = ie;
    }
    k = NFFT;
    while (j >= k) {
      j -= k;
      k = k >> 1;
    }
    j += k;
  }
}


void ram(void) {

  int counter = 0;
  int scale = 8;
  int offset = 0;
	int y,x;
  int channel = ADC_CR_CH7;

	uint8_t adc_val;

	getInputWaitRelease();
	lcdClear(0x00);

	SETUPgout(MIC_AMP_DIS);
	OFF(MIC_AMP_DIS); // Enable AMP
	dac_init(false);

	while(1) {
		while (getInputRaw()==BTN_NONE) {
			if (counter > NFFT) {
        lcd_select();

        for (x=0; x < 130; x++) {
          for (y=0; y < 130; y++) {
            // Clear
            lcdSetPixel(x, y, RGB(0x00,0x00,0x00));
          }
        }

        for (x=0; x < 130; x++) {
          // Cross
      		lcdSetPixel(64, x, RGB(0xFF,0x00,0xFF));
      		lcdSetPixel(x, 64, RGB(0xFF,0x00,0xFF));
      	}

        fft(data);

        for (x=0; x < 130; x++) {
          //lcdSetPixel(x, (data[(x + offset) * 2] / scale) - 1, RGB(0xFF,0xFF,0xFF));
          lcdSetPixel(x, data[(x + offset) * 2] / scale, RGB(0xFF,0xFF,0xFF));
          //lcdSetPixel(x, (data[(x + offset) * 2] / scale) + 1, RGB(0xFF,0xFF,0xFF));
      	}

        lcd_deselect();

        lcdDisplay();

        // delayms(10);

        counter = 0;
      } else {
        data[counter * 2] = adc_get_single(ADC0,channel);
        data[(counter * 2) + 1] = 0;

        //data[counter * 2] = 30;
        counter++;
      }
      delay(0);
		}
		if(getInputRaw() == BTN_ENTER) {
			return;
		}
		if(getInputRaw() == BTN_LEFT) {
      offset++;
      delayms(50);
		}
	}
}
