#include <msp430.h>
#include "state_machines.h"
#include "buzzer.h"
#include "updates.h"
#include "environment.h"
#include "characters.h"
#include "clock.h"
#include "hud.h"

State current_state = INTRO;

void state_intro(){
    intro_buzz();
    display_intro();
}

void state_waiting(){
    display_clock();
}

void state_change_time(){
    blink_change_clock();
}

void state_control_page_one(){
    display_controls();
}
void state_control_page_two(){
    display_controls_two();
}

void state_game(){
    move_clouds();
    move_enemy();
    if (jump_flag){
        character_jump();
        jump_buzz();
    }
    display_score();
}

void state_pause(){
    display_pause();
    reset_pause();
}

void state_game_over(){
    draw_game_over();
    if (buzz_flag)
        buzz_game_over();
}

void transition(State next_state){
    update_vars();
    current_state = next_state;
}
