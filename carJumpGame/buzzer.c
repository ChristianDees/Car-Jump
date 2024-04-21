#include <msp430.h>
#include "libTimer.h"
#include <stdlib.h>
#include "buzzer.h"

int buzz_seconds = 0;           // number of times function is called during each interrupt
char buzz_second_count = 0;     // count of buzz_seconds var
char buzz_toggler = 0;          // char toggler indicating when buzzer should be on/off
char buzz_flag = 0;

void buzzer_init()
{
    timerAUpmode();             // set timer A in up mode for PWM
    P2SEL2 &= ~(BIT6 | BIT7);   // clear P2SEL2.6 and 7 for pin config
    P2SEL &= ~BIT7;             // clear P2SEL.7 for config
    P2SEL |= BIT6;              // set P2SEL.6 for timer A output
    P2DIR = BIT6;               // set P2.6 direction as output for driving the buzzer
}

// set period of the PWM signal
// this buzzer clock is 2MHz
void buzzer_set_period(short cycles)
{
  // capture compare registers
  CCR0 = cycles;        // set period of PWM (how often PWM repeats itself)
  CCR1 = cycles >> 1;   // signal is on same time it is off (making a stable waveform)
}

void buzz_game_over(){
    buzz_seconds++;
    if (buzz_seconds >= 50) {      // once every 90th of a second
        buzz_seconds = 0;
        buzz_second_count++;
        
        buzz_toggler ^= 1;         // "toggle buzzer on/off"
        buzzer_set_period(8000);
        if (!(buzz_toggler))       // if buzzer is not on, set frequency to 0
          buzzer_set_period(0);
        if (buzz_second_count==3){ // on second buzz, make lower pitch
            buzzer_set_period(15000);
        }
        if (buzz_second_count==4){ // on second buzz, make lower pitch
            buzz_flag = 0;
        }
        
    }
}

int jump_buzz_seconds = 0;
char jump_buzz_flag = 0;
void jump_buzz(){
    if(jump_buzz_flag && !buzz_flag){
        jump_buzz_seconds++;
        if(jump_buzz_seconds >= 10){
            buzzer_set_period(5000);
        }
        if(jump_buzz_seconds >= 50){
            buzzer_set_period(0);
            jump_buzz_seconds = 0;
            jump_buzz_flag = 0;
        }
    }
}

int intro_buzz_seconds = 0;
void intro_buzz(){
    intro_buzz_seconds++;
    if (intro_buzz_seconds < 125 && intro_buzz_seconds >= 10)
        buzzer_set_period(15000);
    else if (intro_buzz_seconds < 250 && intro_buzz_seconds > 125)
        buzzer_set_period(10000);
    else if (intro_buzz_seconds < 375 && intro_buzz_seconds > 250 && intro_buzz_seconds > 125)
        buzzer_set_period(5000);
    else if (intro_buzz_seconds >= 375)
        buzzer_set_period(0);
}
