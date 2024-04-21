#include <msp430.h>
#include <libTimer.h>
#include "state_machines.h"

void wdt_c_handler()
{
    switch (current_state) {
        case WAITING:
            state_waiting();
            break;
        case CHANGETIME:
            state_change_time();
            break;
        case CONTROLPAGEONE:
            state_control_page_one();
            break;
        case CONTROLPAGETWO:
            state_control_page_two();
            break;
        case GAME:
            state_game();
            break;
        case PAUSE:
            state_pause();
            break;
        case GAMEOVER:
            state_game_over();
            break;
    }
}
