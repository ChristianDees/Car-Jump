#include <msp430.h>
#include "switches.h"
#include "updates.h"
#include "state_machines.h"
#include "characters.h"
#include "hud.h"
#include "clock.h"
#include "buzzer.h"


char
switch_update_interrupt_sense()
{
  char p2val = P2IN;
  P2IES |= (p2val & SWITCHES);    // if switch up, sense down
  
  return p2val;
}

// setup switches
void
switch_init()
{
  P2REN |= SWITCHES;        // enables resistors for switches
  P2IE |= SWITCHES;         // enable interrupts from switches
  P2OUT |= SWITCHES;        // pull-ups for switches
  P2DIR &= ~SWITCHES;       // set switches' bits for input
}

// handler for each button
void
switch_interrupt_handler()
{
    char p2val = switch_update_interrupt_sense();
    
    // button inputs
    char button1 = (p2val & SW1) ? 0 : 1;
    char button2 = (p2val & SW2) ? 0 : 1;
    char button3 = (p2val & SW3) ? 0 : 1;
    char button4 = (p2val & SW4) ? 0 : 1;
    
    // control what buttons allowed in each state
    switch(current_state){
        case WAITING:
            if (button1)
                transition(GAME);
            else if (button4)
                current_state = CHANGETIME;
            else if (button3)
                transition(CONTROLPAGEONE);
            break;
        case CHANGETIME:
            if (button2){
                changeTime = 1;
                hour++;
                update_time(1,0);
            } else if (button3){
                changeTime = 1;
                minutes++;
                update_time(0,1);
            } else if (button1)
                current_state = WAITING;
            break;
        case CONTROLPAGEONE:
            if(button1)
                transition(GAME);
            else if(button2)
                transition(WAITING);
            else if(button3)
                transition(CONTROLPAGETWO);
            break;
        case CONTROLPAGETWO:
            if(button2)
                transition(CONTROLPAGEONE);
            break;
        case GAME:
            if (button2){
                jump_flag = 1;
                jump_buzz_flag = 1;
            }
            else if (button3)
                pause_flag = 1;
            break;
        case PAUSE:
            if (button3)
                reset_pause_flag = 1;
            else if (button4)
                transition(WAITING);
            break;
        case GAMEOVER:
            if (button1)
                transition(GAME);
            else if (button4)
                transition(WAITING);
            break;
    }
}
