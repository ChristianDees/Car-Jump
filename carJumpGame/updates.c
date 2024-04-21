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

char redrawScreen = 0;
    
void
update_shapes()
{
    if(current_state == GAME){
        screen_update_score();
        screen_update_ball(drawPos_one, controlPos_one);
        screen_update_ball(drawPos_two, controlPos_two);
        screen_update_road(drawPos_road_one, controlPos_road_one);
        screen_update_road(drawPos_road_two, controlPos_road_two);
        floor_bar();
        screen_update_enemy(colorBGR);
        if (enable_second_enemy)
            screen_update_enemy_two(colorBGR_two);
        screen_update_character();
        if(overlap_flag)
            current_state = GAMEOVER;
        if (pause_flag){
            pause_flag = 0;
            current_state = PAUSE;
        }
    }
}

void update_vars(){
    clearScreen(COLOR_GRAY);
    int x = random_int_generator();
    if (x == 1){
        colorBGR = COLOR_GREEN;
        colorBGR_two = COLOR_GREEN;
    } else if (x == 2){
        colorBGR = COLOR_RED;
        colorBGR_two = COLOR_RED;
    } else if (x == 3){
        colorBGR = COLOR_PURPLE;
        colorBGR_two = COLOR_PURPLE;
    } else if (x == 4){
        colorBGR = COLOR_ORANGE;
        colorBGR_two = COLOR_ORANGE;
    }
    
    count = 0;
    count2 = 0;
    secCount = 0;
    totalSeconds = 0;
    
    secCounOne = 0;
    drawPos_enemy[0] = 60;
    drawPos_enemy[1] = screenHeight/2 +24;
    controlPos_enemy[0] = 61;
    controlPos_enemy[1] = screenHeight/2 +24;
    
    drawPos_enemy_two[0] = 130;
    drawPos_enemy_two[1] = screenHeight/2 +24;
    controlPos_enemy_two[0] = 131;
    controlPos_enemy_two[1] = screenHeight/2 +24;
    
    drawPos_road_one[0] = 0;
    drawPos_road_one[1] = screenHeight/2 +30;
    controlPos_road_one[0] = 1;
    controlPos_road_one[1] = screenHeight/2 +30;
    
    drawPos_road_two[0] = 85;
    drawPos_road_two[1] = screenHeight/2 +30;
    controlPos_road_two[0] = 86;
    controlPos_road_two[1] = screenHeight/2 +30;
    
    drawPosCharacter[0] = 10;
    drawPosCharacter[1] = screenHeight/2 +25;
    controlPosCharacter[0] = 10;
    controlPosCharacter[1] = screenHeight/2 +24;
    
    drawPos_one[0] = screenWidth/2 +60;
    drawPos_one[1] = 10;
    controlPos_one[0] = screenWidth/2 +59;
    controlPos_one[1] = 10;
    
    drawPos_two[0] = screenWidth/2 +1;
    drawPos_two[1] = 30;
    controlPos_two[0] = screenWidth/2;
    controlPos_two[1] = 30;
    
    buzz_seconds = 0;
    buzz_toggler = 0;
    buzz_second_count = 0;
    buzz_flag = 0;
    buzzer_set_period(0);
    
    floor_done = 1;
    jump_flag = 0;
    draw_once = 1;
    score_once = 1;
    current_score = 0;
    display_pause_once = 1;
    enable_second_enemy = 0;
    overlap_flag = 0;
    seconds = 0;
    display_controls_once = 1;
    display_clock_once = 1;
    jump_buzz_flag = 0;
    jump_buzz_seconds = 0;
}
