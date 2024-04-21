#include "lcdutils.h"
#include "lcddraw.h"
#include "updates.h"
#include "int_to_str.h"
#include "state_machines.h"
#include "buzzer.h"

int high_score = 0;
int current_score = 0;
char display_pause_once = 1;
char reset_pause_flag = 0;
char display_controls_once = 1;

char score_once = 1;
char seconds_score = 0;

char draw_once = 1;
char pause_flag = 0;

char display_intro_once = 1;


int introSeconds = 0;

void display_intro(){
    
    if (display_intro_once){
        clearScreen(COLOR_RED);
        fillRectangle(15, 15, screenWidth-30, 4, WHITE);
        drawString11x16_normal(45,60, "CAR", WHITE, COLOR_RED);
        drawString11x16_normal(40,85, "JUMP", WHITE, COLOR_RED);
        fillRectangle(15, screenHeight-15, screenWidth-30, 4, WHITE);
        display_intro_once = 0;
    }
    introSeconds++;
    if(introSeconds >= 375)
        transition(WAITING);
}

















void display_controls(){
    if (display_controls_once){
        drawString5x7(5,5, "< SW2", BLACK, COLOR_GRAY);
        drawString5x7(screenWidth-30,5, "SW3 >", BLACK, COLOR_GRAY);
        drawString11x16_normal(20,20, "Controls", BLACK, COLOR_GRAY);
        fillRectangle(15, 40, screenWidth-30, 2, BLACK);
        
        drawString8x12(17,55, "SW1:", BLACK, COLOR_GRAY);
        drawString8x12(55,55, "Start", BLACK, COLOR_GRAY);
        
        drawString8x12(17,80, "SW2:", BLACK, COLOR_GRAY);
        drawString8x12(55,80, "Jump", BLACK, COLOR_GRAY);
        
        drawString8x12(17,105, "SW3:", BLACK, COLOR_GRAY);
        drawString8x12(55,105, "Pause", BLACK, COLOR_GRAY);
        
        drawString8x12(17,130, "SW4:", BLACK, COLOR_GRAY);
        drawString8x12(55,130, "Set Time", BLACK, COLOR_GRAY);
        
        redrawScreen = 1;
        display_controls_once = 0;

    }
}

void display_controls_two(){
    if (display_controls_once){
        drawString5x7(5,5, "< SW2", BLACK, COLOR_GRAY);
        drawString11x16_normal(40,20, "Time", BLACK, COLOR_GRAY);
        fillRectangle(15, 40, screenWidth-30, 2, BLACK);
        
        drawString8x12(17,55, "SW1:", BLACK, COLOR_GRAY);
        drawString8x12(55,55, "Save", BLACK, COLOR_GRAY);
        
        drawString8x12(17,80, "SW2:", BLACK, COLOR_GRAY);
        drawString8x12(55,80, "Hours", BLACK, COLOR_GRAY);
        
        drawString8x12(17,105, "SW3:", BLACK, COLOR_GRAY);
        drawString8x12(55,105, "Minutes", BLACK, COLOR_GRAY);

        drawString8x12(17,130, "SW4:", BLACK, COLOR_GRAY);
        drawString8x12(55,130, "Return", BLACK, COLOR_GRAY);
        
        redrawScreen = 1;
        display_controls_once = 0;
    }
}





void display_score(){
    if (score_once){
        char str[20];
        int_to_str(str, high_score, 3);
        drawString8x12(40, 135, str, BLACK, COLOR_GRAY);
        drawString8x12(10, 135, "HI:", BLACK, COLOR_GRAY);
        redrawScreen=1;
        score_once = 0;
    }
    seconds_score++;
    if (seconds_score >= 250){
        seconds_score = 0;
        current_score++;
    }
    
}


void screen_update_score(){
    if (current_score > high_score)
        high_score = current_score;
    char str[20]; // Adjust the size as needed
    int_to_str(str, current_score, 3);
    drawString8x12(90, 135, str, BLACK, COLOR_GRAY);
    
}


void display_pause(){
    if (display_pause_once){
        short centerCol = screenWidth >> 1;
        short centerRow = screenHeight >> 1;
        short size = 40;
        short startCol = centerCol - size / 2; // Start from the rightmost point
        short startRow = centerRow - (size / 2) - 15;
        drawRectOutline(startCol, startRow, size, size, BLACK);
        fillRectangle(startCol+3, startRow+3, size-6, size-6, WHITE);
        fillRectangle(centerCol-6, startRow+8, 5, 24, BLACK);
        fillRectangle(centerCol+1, startRow+8, 5, 24, BLACK);
        display_pause_once = 0;
        redrawScreen = 1;
    }
}


void reset_pause(){
    if (reset_pause_flag){
        fillRectangle((screenWidth >> 1)-(40/2), (screenHeight >> 1)-(40/2)-15, 40, 40, COLOR_GRAY);
        current_state = GAME;
        reset_pause_flag = 0;
        redrawScreen = 1;
        display_pause_once = 1;
    }
}



void draw_game_over() {
    if (draw_once){
        buzz_flag = 1;
        fillRectangle(30, screenWidth/2 -15, 65, 45, COLOR_BLACK);
        drawString11x16(35,screenWidth/2 -10, "Game", COLOR_RED, BLACK);
        drawString11x16(35,screenWidth/2 +10, "Over", COLOR_RED, BLACK);
        draw_once = 0;
        redrawScreen = 1;
    }
    
}


