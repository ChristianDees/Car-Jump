#include <msp430.h>
#include "lcdutils.h"
#include "lcddraw.h"
#include "updates.h"
#include "random_int.h"
#include "hud.h"


char enable_second_enemy = 0;
char overlap_flag = 0;
char jump_flag = 0;

short drawPos_enemy[2] = {60,screenHeight/2 +24}, controlPos_enemy[2] = {61,screenHeight/2 +24};
short colEnemyVelocity = -10;

short drawPos_enemy_two[2] = {130,screenHeight/2 +24}, controlPos_enemy_two[2] = {131,screenHeight/2 +24};
short colEnemyVelocity_two = -10;

short drawPosCharacter[2] = {10,screenHeight/2 +25};
short controlPosCharacter[2] = {10,screenHeight/2 +24};

short colVelocityCharacter = 12;
short colLimitsCharacter[2] = {(screenHeight/2) +25 -60, (screenHeight/2) +25};

short enemy_seconds = 0;
short jump_seconds = 0;
short post_secs = 0;
short jump_once = 0;
u_int colorBGR;
u_int colorBGR_two;


char do_overlap(char cTLX, char cTLY, char cBRX, char cBRY, char eTLX, char eTLY, char eBRX, char eBRY) {
    // rectangle on left
    if (cBRX < eTLX || eBRX < cTLX)
        return 0;
    // rectangle above
    if (cBRY < eTLY || eBRY < cTLY)
        return 0;
    // else, they overlap
    return 1;
}

void move_enemy() {
    enemy_seconds++;
    if (enemy_seconds >= 50){ // every 50th of a second
        post_secs++;
        enemy_seconds = 0;
        short oldCol_enemy = controlPos_enemy[0];
        short newCol_enemy = oldCol_enemy + colEnemyVelocity;
        // check if enemy is completely off screen
        if (newCol_enemy + 30 < 0) {
            // if off screen, reset color and position
            int x = random_int_generator();
            if (x == 1){
                colorBGR = COLOR_GREEN;
            } else if (x == 2){
                colorBGR = COLOR_RED;
            } else if (x == 3){
                colorBGR = COLOR_PURPLE;
            } else if (x == 4){
                colorBGR = COLOR_ORANGE;
            }
            controlPos_enemy[0] = screenWidth;
            post_secs = 0;
        } else
            controlPos_enemy[0] = newCol_enemy;
        // if one jump is completed
        if (current_score >= 10 && post_secs ==8){
            post_secs = 0;
            enable_second_enemy = 1;
        }
        // move second enemy if enabled
        if(enable_second_enemy){
            short oldCol_enemy_two = controlPos_enemy_two[0];
            short newCol_enemy_two = oldCol_enemy_two + colEnemyVelocity_two;
            if (newCol_enemy_two + 30 < 0) {
                int x = random_int_generator();
                if (x == 1)
                    colorBGR_two = COLOR_GREEN;
                else if (x == 2)
                    colorBGR_two = COLOR_RED;
                else if (x == 3)
                    colorBGR_two = COLOR_PURPLE;
                else if (x == 4)
                    colorBGR_two = COLOR_ORANGE;
                controlPos_enemy_two[0] = screenWidth;
            } else
                controlPos_enemy_two[0] = newCol_enemy_two;
        }
        // check if anything is overlapping
        if (do_overlap(drawPosCharacter[0], drawPosCharacter[1]-11, drawPosCharacter[0]+16, drawPosCharacter[1],drawPos_enemy[0]+3, drawPos_enemy[1]-11, drawPos_enemy[0]+16, drawPos_enemy[1]))
            overlap_flag = 1;
        if (do_overlap(drawPosCharacter[0], drawPosCharacter[1]-11, drawPosCharacter[0]+16, drawPosCharacter[1],drawPos_enemy_two[0]+3, drawPos_enemy_two[1]-11, drawPos_enemy_two[0]+16, drawPos_enemy_two[1]))
            overlap_flag = 1;
        if (do_overlap(drawPosCharacter[0]-3, drawPosCharacter[1]-11, drawPosCharacter[0]+16, drawPosCharacter[1],drawPos_enemy[0], drawPos_enemy[1]-11, drawPos_enemy[0]+16, drawPos_enemy[1]))
            overlap_flag = 1;
        if (do_overlap(drawPosCharacter[0]-3, drawPosCharacter[1]-11, drawPosCharacter[0]+16, drawPosCharacter[1],drawPos_enemy_two[0], drawPos_enemy_two[1]-11, drawPos_enemy_two[0]+16, drawPos_enemy_two[1]))
            overlap_flag = 1;
        redrawScreen = 1;  // mark screen for redrawing
        enemy_seconds = 0;       // reset second count
    }
}

void
draw_character(int col, int row, unsigned short color1, unsigned short color2, unsigned short color3, unsigned short color4)
{
    fillRectangle(col+3, row, 4,2, color1);
    fillRectangle(col+12, row, 4,2, color1);
    fillRectangle(col, row-1, 20, 1,color2);
    fillRectangle(col, row-2, 20, 1,color3);
    fillRectangle(col, row-3, 20, 1,color3);
    fillRectangle(col, row-4, 20, 2,color3);
    fillRectangle(col, row-6, 20, 2,color2);
    fillRectangle(col+4, row-11, 10, 5, color2);
    fillRectangle(col+7, row-9, 5, 3, color4);
}

void
screen_update_character()
{
  for (char axis = 0; axis < 2; axis ++)
    if (drawPosCharacter[axis] != controlPosCharacter[axis])
      goto redraw;
  return;
 redraw:
    if (drawPosCharacter[1] <= screenHeight/2+25 && drawPosCharacter[1] >= screenHeight/2 +26 -4){
        draw_character(drawPosCharacter[0], drawPosCharacter[1], COLOR_GRAY,COLOR_GRAY,COLOR_BLACK, COLOR_GRAY); // erase
    }else{
        draw_character(drawPosCharacter[0], drawPosCharacter[1], COLOR_GRAY,COLOR_GRAY,COLOR_GRAY, COLOR_GRAY); // erase
    }
  for (char axis = 0; axis < 2; axis ++)
    drawPosCharacter[axis] = controlPosCharacter[axis];
  draw_character(drawPosCharacter[0], drawPosCharacter[1], COLOR_BLACK, COLOR_BLUE, COLOR_BLUE, WHITE); // draw
}

void 
character_jump() {
    jump_seconds ++;
     if (jump_seconds >= 25) {
         jump_once++;
         short oldRow = controlPosCharacter[1];
         short newRow = oldRow - colVelocityCharacter;
         if (newRow <= colLimitsCharacter[0] || newRow >= colLimitsCharacter[1] )
            colVelocityCharacter = -colVelocityCharacter;
         else
             controlPosCharacter[1] = newRow;
         if (newRow == screenHeight/2 +24 && jump_once){
             jump_flag = 0;
             jump_once = 0;
         }
         if (do_overlap(drawPosCharacter[0], drawPosCharacter[1]-11, drawPosCharacter[0]+16, drawPosCharacter[1],drawPos_enemy[0]+3, drawPos_enemy[1]-11, drawPos_enemy[0]+16, drawPos_enemy[1]))
             overlap_flag = 1;
         if (do_overlap(drawPosCharacter[0], drawPosCharacter[1]-11, drawPosCharacter[0]+16, drawPosCharacter[1],drawPos_enemy_two[0]+3, drawPos_enemy_two[1]-11, drawPos_enemy_two[0]+16, drawPos_enemy_two[1]))
             overlap_flag = 1;
         if (do_overlap(drawPosCharacter[0]-3, drawPosCharacter[1]-11, drawPosCharacter[0]+16, drawPosCharacter[1],drawPos_enemy[0], drawPos_enemy[1]-11, drawPos_enemy[0]+16, drawPos_enemy[1]))
             overlap_flag = 1;
         if (do_overlap(drawPosCharacter[0]-3, drawPosCharacter[1]-11, drawPosCharacter[0]+16, drawPosCharacter[1],drawPos_enemy_two[0], drawPos_enemy_two[1]-11, drawPos_enemy_two[0]+16, drawPos_enemy_two[1]))
             overlap_flag = 1;
         jump_seconds = 0;
         redrawScreen = 1;
    }
}

void
draw_enemy(int col, int row, unsigned short color1, unsigned short color2, unsigned short color3, unsigned short color4)
{
    // for loops to only print if still on screen
    for(char i = 0; i < 20; i++)
        if (col+i >= 0)
            fillRectangle(col+i,row-4, 1, 2, color3);
    for(char i = 0; i < 20; i++)
        if (col+i >= 0)
            fillRectangle(col+i,row-6, 1, 2, color2);
    for(char i = 0; i < 10; i++)
        if (col+6+i >= 0)
            fillRectangle(col+6+i,row-11, 1, 5, color2);
    for(char i = 0; i < 5; i++)
        if (col+8+i >= 0)
            fillRectangle(col+8+i,row-9, 1, 3, color4);
    for(char i = 0; i < 4; i++)
        if (col+3+i >= 0)
            fillRectangle(col+3+i,row, 1, 2, color1);
    for(char i = 0; i < 4; i++)
        if (col+12+i >= 0)
            fillRectangle(col+12+i,row, 1, 2, color1);
    for(char i = 0; i < 20; i++)
        if (col+i >= 0)
            fillRectangle(col+i,row-1, 1, 1, color2);
    for(char i = 0; i < 20; i++)
        if (col+i >= 0)
            fillRectangle(col+i,row-2, 1, 1, color3);
    for(char i = 0; i < 20; i++)
        if (col+i >= 0)
            fillRectangle(col+i,row-3, 1, 1, color3);
}

void
screen_update_enemy(u_int colorBGR)
{
    for (char axis = 0; axis < 2; axis ++)
        if (drawPos_enemy[axis] != controlPos_enemy[axis])
            goto redraw;
    return;
redraw:
    draw_enemy(drawPos_enemy[0], drawPos_enemy[1], COLOR_GRAY,COLOR_GRAY,COLOR_BLACK, COLOR_GRAY);
    for (char axis = 0; axis < 2; axis ++)
        drawPos_enemy[axis] = controlPos_enemy[axis];
    draw_enemy(drawPos_enemy[0], drawPos_enemy[1], COLOR_BLACK, colorBGR, colorBGR, WHITE);
}

void
screen_update_enemy_two(u_int colorBGR)
{
  for (char axis = 0; axis < 2; axis ++)
    if (drawPos_enemy_two[axis] != controlPos_enemy_two[axis])
      goto redraw;
  return;
 redraw:
  draw_enemy(drawPos_enemy_two[0], drawPos_enemy_two[1], COLOR_GRAY,COLOR_GRAY,COLOR_BLACK, COLOR_GRAY);
  for (char axis = 0; axis < 2; axis ++)
    drawPos_enemy_two[axis] = controlPos_enemy_two[axis];
  draw_enemy(drawPos_enemy_two[0], drawPos_enemy_two[1], COLOR_BLACK, colorBGR, colorBGR, WHITE);
}
