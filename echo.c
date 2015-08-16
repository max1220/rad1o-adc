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

#include <fatfs/ff.h>

#include "usetable.h"

void ram(void) {
  getInputWaitRelease();
  lcdClear();
  lcdPrintln("DAC test");
  lcdNl();
  lcdPrintln("Up Load file");
  lcdPrintln("Lt File output");
  lcdPrintln("Rt Echo test");
  lcdDisplay();

  SETUPgout(MIC_AMP_DIS);
  OFF(MIC_AMP_DIS); // Enable AMP
  dac_init(false);

  while(1) {
    if (getInputRaw()==BTN_NONE) {
      dac_set(adc_get_single(ADC0,ADC_CR_CH7)>>1);
    }
    else {
      return;
    }
  }

}
