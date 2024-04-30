#include <msp430.h>
#include <libTimer.h>
#include "state_machines.h"

void wdt_c_handler()
{
    count_time();
    switch (current_state) {
        case INTRO:
            state_intro();
            break;
        case CHANGETIME:
            state_change_time();
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
