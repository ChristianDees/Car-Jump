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

short count2 = 0;
short seconds = 0;

short secCounOne = 0;
short secCountTwo = 0;

u_int colorBGR;
u_int colorBGR_two;


char do_overlap(char cTLX, char cTLY, char cBRX, char cBRY, char eTLX, char eTLY, char eBRX, char eBRY) {
    // Check if one rectangle is on the left side of the other
    if (cBRX < eTLX || eBRX < cTLX)
        return 0;
    // Check if one rectangle is above the other
    if (cBRY < eTLY || eBRY < cTLY)
        return 0;
    
    // If none of the above conditions are met, there's an overlap
    return 1;
}


void move_enemy() {
    

    P1OUT |= 64;  // Green LED on when CPU on
    
    count2++;
    
    if (count2 >= 50){
        seconds++;
        count2=0;
        short oldCol_enemy = controlPos_enemy[0];
        short newCol_enemy = oldCol_enemy + colEnemyVelocity;  // colVelocity should be negative

        // Check if the ball is completely off-screen on the left
        if (newCol_enemy + 30 < 0) {  // BALL_WIDTH is the width of the ball
            // If off-screen, reset its position to just off-screen on the right
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
            controlPos_enemy[0] = screenWidth;  // Start from the right side again
            seconds = 0;
            // You can adjust the vertical position randomly or keep it static as needed
            // Example for random vertical position within limits:
            /*controlPos[1] = 50; */ // Adjust range according to screen height
        } else {
            // Otherwise, update the position as before
            
            controlPos_enemy[0] = newCol_enemy;
            
        }
        
        
        if (current_score >= 10 && seconds ==8){
            seconds = 0;
            enable_second_enemy = 1;
        }
            
        if(enable_second_enemy){
            short oldCol_enemy_two = controlPos_enemy_two[0];
            short newCol_enemy_two = oldCol_enemy_two + colEnemyVelocity_two;  // colVelocity should be negative
            
            // Check if the ball is completely off-screen on the left
            if (newCol_enemy_two + 30 < 0) {  // BALL_WIDTH is the width of the ball
                // If off-screen, reset its position to just off-screen on the right
                int x;
                if (current_score > high_score)
                    x = (current_score - high_score) + 1;
                else
                    x = (high_score - current_score) + 1;
                x = x%4;
                if (x == 1){
                    colorBGR_two = COLOR_GREEN;
                } else if (x == 2){
                    colorBGR_two = COLOR_RED;
                } else if (x == 3){
                    colorBGR_two = COLOR_PURPLE;
                } else if (x == 4){
                    colorBGR_two = COLOR_ORANGE;
                }
                controlPos_enemy_two[0] = screenWidth;  // Start from the right side again
                
                // You can adjust the vertical position randomly or keep it static as needed
                // Example for random vertical position within limits:
                /*controlPos[1] = 50; */ // Adjust range according to screen height
            } else {
                // Otherwise, update the position as before
                
                controlPos_enemy_two[0] = newCol_enemy_two;
            }
            
    
        }
        if (do_overlap(drawPosCharacter[0], drawPosCharacter[1]-11, drawPosCharacter[0]+16, drawPosCharacter[1],drawPos_enemy[0]+3, drawPos_enemy[1]-11, drawPos_enemy[0]+16, drawPos_enemy[1]))
            overlap_flag = 1;
        if (do_overlap(drawPosCharacter[0], drawPosCharacter[1]-11, drawPosCharacter[0]+16, drawPosCharacter[1],drawPos_enemy_two[0]+3, drawPos_enemy_two[1]-11, drawPos_enemy_two[0]+16, drawPos_enemy_two[1]))
            overlap_flag = 1;
        if (do_overlap(drawPosCharacter[0]-3, drawPosCharacter[1]-11, drawPosCharacter[0]+16, drawPosCharacter[1],drawPos_enemy[0], drawPos_enemy[1]-11, drawPos_enemy[0]+16, drawPos_enemy[1]))
            overlap_flag = 1;
        if (do_overlap(drawPosCharacter[0]-3, drawPosCharacter[1]-11, drawPosCharacter[0]+16, drawPosCharacter[1],drawPos_enemy_two[0], drawPos_enemy_two[1]-11, drawPos_enemy_two[0]+16, drawPos_enemy_two[1]))
            overlap_flag = 1;
        
        redrawScreen = 1;  // Mark the screen for redrawing
        count2 = 0;
    }
    P1OUT &= ~64;  // Turn off the LED
}


void
draw_character(int col, int row, unsigned short color1, unsigned short color2, unsigned short color3, unsigned short color4)
{
//    fillRectangle(col+3, row, 4,2, color1); //wheel left
//    fillRectangle(col+12, row, 4,2, color1); //wheel right
//    fillRectangle(col, row-6, 20, 6, color2); //bottom of car
//    fillRectangle(col+4, row-11, 10, 5, color2); // top of car
//    fillRectangle(col+7, row-9, 5, 4, color3); // top of car
//
//
    
    fillRectangle(col+3, row, 4,2, color1); //wheel left
    fillRectangle(col+12, row, 4,2, color1); //wheel right
    
    fillRectangle(col, row-1, 20, 1,color2);
    fillRectangle(col, row-2, 20, 1,color3);

    fillRectangle(col, row-3, 20, 1,color3);
    
    fillRectangle(col, row-4, 20, 2,color3);
    
    fillRectangle(col, row-6, 20, 2,color2);

    
    fillRectangle(col+4, row-11, 10, 5, color2); // top of car
    fillRectangle(col+7, row-9, 5, 3, color4); // top of car
    
    
    // color 1 = wheels = gray
    
    
}

void
screen_update_character()
{
  for (char axis = 0; axis < 2; axis ++)
    if (drawPosCharacter[axis] != controlPosCharacter[axis]) /* position changed? */
      goto redraw;
  return;            /* nothing to do */
 redraw:
    
    // if row is equal to where road is
    // if current row between screenHeight/2+20 and screenHeight/2+20+3
    if (drawPosCharacter[1] <= screenHeight/2+25 && drawPosCharacter[1] >= screenHeight/2 +26 -4){
        draw_character(drawPosCharacter[0], drawPosCharacter[1], COLOR_GRAY,COLOR_GRAY,COLOR_BLACK, COLOR_GRAY); /* erase */
    }else{
        draw_character(drawPosCharacter[0], drawPosCharacter[1], COLOR_GRAY,COLOR_GRAY,COLOR_GRAY, COLOR_GRAY); /* erase */
    }
  for (char axis = 0; axis < 2; axis ++)
    drawPosCharacter[axis] = controlPosCharacter[axis];
  draw_character(drawPosCharacter[0], drawPosCharacter[1], COLOR_BLACK, COLOR_BLUE, COLOR_BLUE, WHITE); /* draw */
}


void character_jump() {
    
    
    secCounOne ++;
     if (secCounOne >= 25) {        /* 10/sec */
         secCountTwo++;
                     /* move ball */
         
         short oldRow = controlPosCharacter[1];
         short newRow = oldRow - colVelocityCharacter;
         
         if (newRow <= colLimitsCharacter[0] || newRow >= colLimitsCharacter[1] )
            colVelocityCharacter = -colVelocityCharacter;
         else
             controlPosCharacter[1] = newRow;
       
         if (newRow == screenHeight/2 +24 && secCountTwo){
             
             jump_flag = 0;
             secCountTwo = 0;
             

         }
         if (do_overlap(drawPosCharacter[0], drawPosCharacter[1]-11, drawPosCharacter[0]+16, drawPosCharacter[1],drawPos_enemy[0]+3, drawPos_enemy[1]-11, drawPos_enemy[0]+16, drawPos_enemy[1]))
             overlap_flag = 1;
         if (do_overlap(drawPosCharacter[0], drawPosCharacter[1]-11, drawPosCharacter[0]+16, drawPosCharacter[1],drawPos_enemy_two[0]+3, drawPos_enemy_two[1]-11, drawPos_enemy_two[0]+16, drawPos_enemy_two[1]))
             overlap_flag = 1;
         if (do_overlap(drawPosCharacter[0]-3, drawPosCharacter[1]-11, drawPosCharacter[0]+16, drawPosCharacter[1],drawPos_enemy[0], drawPos_enemy[1]-11, drawPos_enemy[0]+16, drawPos_enemy[1]))
             overlap_flag = 1;
         if (do_overlap(drawPosCharacter[0]-3, drawPosCharacter[1]-11, drawPosCharacter[0]+16, drawPosCharacter[1],drawPos_enemy_two[0], drawPos_enemy_two[1]-11, drawPos_enemy_two[0]+16, drawPos_enemy_two[1]))
             overlap_flag = 1;
         
        redrawScreen = 1;  // Mark the screen for redrawing
        
         secCounOne = 0;
    }
    
}

//2
//2
//2


void
draw_enemy(int col, int row, unsigned short color1, unsigned short color2, unsigned short color3, unsigned short color4)
{
    
    
    
//    fillRectangle(col+3, row, 4,2, color1); //wheel left
//    fillRectangle(col+12, row, 4,2, color1); //wheel right
//
//    fillRectangle(col, row-1, 20, 1,color2);
//    fillRectangle(col, row-2, 20, 1,color3);
//
//    fillRectangle(col, row-3, 20, 1,color3);
//
//    fillRectangle(col, row-4, 20, 2,color3);
//
//    fillRectangle(col, row-6, 20, 2,color2);
//
//
//    fillRectangle(col+4, row-11, 10, 5, color2); // top of car
//    fillRectangle(col+7, row-9, 5, 3, color4); // top of car
//
    
    
    
    for(char i = 0; i < 20; i++){
        if (col+i >= 0){
            fillRectangle(col+i,row-4, 1, 2, color3);
        }
    }
    for(char i = 0; i < 20; i++){
        if (col+i >= 0){
            fillRectangle(col+i,row-6, 1, 2, color2);
        }
    }
    
    for(char i = 0; i < 10; i++){
        if (col+6+i >= 0){
            fillRectangle(col+6+i,row-11, 1, 5, color2);
        }
    }
    
    for(char i = 0; i < 5; i++){
        if (col+8+i >= 0){
            fillRectangle(col+8+i,row-9, 1, 3, color4);
        }
    }
    
    for(char i = 0; i < 4; i++){
        if (col+3+i >= 0){
            fillRectangle(col+3+i,row, 1, 2, color1);
        }
    }
    for(char i = 0; i < 4; i++){
        if (col+12+i >= 0){
            fillRectangle(col+12+i,row, 1, 2, color1);
        }
    }
    
    for(char i = 0; i < 20; i++){
        if (col+i >= 0){
            fillRectangle(col+i,row-1, 1, 1, color2);
        }
    }
    for(char i = 0; i < 20; i++){
        if (col+i >= 0){
            fillRectangle(col+i,row-2, 1, 1, color3);
        }
    }
    
    for(char i = 0; i < 20; i++){
        if (col+i >= 0){
            fillRectangle(col+i,row-3, 1, 1, color3);
        }
    }
    
    
}
// wheels = color 1
// bottom portion of car = gray for one layer
// bottom portion two = black
// top portion of car = gray

// color1 = gray
// color1 = gray
// color2 = black
// color

  

void
screen_update_enemy(u_int colorBGR)
{
  for (char axis = 0; axis < 2; axis ++)
    if (drawPos_enemy[axis] != controlPos_enemy[axis]) /* position changed? */
      goto redraw;
  return;            /* nothing to do */
 redraw:
  draw_enemy(drawPos_enemy[0], drawPos_enemy[1], COLOR_GRAY,COLOR_GRAY,COLOR_BLACK, COLOR_GRAY); /* erase */
  for (char axis = 0; axis < 2; axis ++)
    drawPos_enemy[axis] = controlPos_enemy[axis];
    
    
  
  draw_enemy(drawPos_enemy[0], drawPos_enemy[1], COLOR_BLACK, colorBGR, colorBGR, WHITE); /* draw */
}


void
screen_update_enemy_two(u_int colorBGR)
{
  for (char axis = 0; axis < 2; axis ++)
    if (drawPos_enemy_two[axis] != controlPos_enemy_two[axis]) /* position changed? */
      goto redraw;
  return;            /* nothing to do */
 redraw:
  draw_enemy(drawPos_enemy_two[0], drawPos_enemy_two[1], COLOR_GRAY,COLOR_GRAY,COLOR_BLACK, COLOR_GRAY); /* erase */
  for (char axis = 0; axis < 2; axis ++)
    drawPos_enemy_two[axis] = controlPos_enemy_two[axis];
    
    
  
  draw_enemy(drawPos_enemy_two[0], drawPos_enemy_two[1], COLOR_BLACK, colorBGR, colorBGR, WHITE); /* draw */
}









