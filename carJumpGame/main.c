#include <msp430.h>
#include <libTimer.h>
#include <stdio.h>
#include "lcdutils.h"
#include "lcddraw.h"
#include "buzzer.h"
#include "state_machines.h"
#include "random_int.h"
#include "switches.h"
#include "int_to_str.h"
#include "environment.h"
#include "characters.h"
#include "clock.h"
#include "hud.h"
#include "updates.h"

#define LED BIT6


void main()
{
  
  P1DIR |= LED;        /**< Green led on when CPU on */
  P1OUT |= LED;
  configureClocks();
  lcd_init();
  switch_init();
  buzzer_init();
  adc_init();
  enableWDTInterrupts();      /**< enable periodic interrupt */
  or_sr(0x8);                  /**< GIE (enable interrupts) */
  while(1) {
      if (redrawScreen) { /**< Pause CPU if screen doesn't need updating */
          update_shapes();
          redrawScreen = 0;
      }
      P1OUT &= ~LED;    /* led off */
      or_sr(0x10);    /**< CPU OFF */
      P1OUT |= LED;    /* led on */
  }
}

