#include <msp430.h>
#include "lcdutils.h"
#include "lcddraw.h"
#include "updates.h"
short count = 0;
char floor_done = 1;



short drawPos_road_one[2] = {0,screenHeight/2 +30}, controlPos_road_one[2] = {1,screenHeight/2 +30};

short drawPos_road_two[2] = {85,screenHeight/2 +30}, controlPos_road_two[2] = {86,screenHeight/2 +30};


short colRoadVelocity = -10;

void
draw_road(int col, int row, unsigned short color)
{
    fillRectangle(col, row, 30, 2, color);

}

void
screen_update_road(short drawPos[2], short controlPos[2])
{
  for (char axis = 0; axis < 2; axis ++)
    if (drawPos[axis] != controlPos[axis]) /* position changed? */
      goto redraw;
  return;            /* nothing to do */
 redraw:
  draw_road(drawPos[0], drawPos[1], COLOR_GRAY); /* erase */
  for (char axis = 0; axis < 2; axis ++)
    drawPos[axis] = controlPos[axis];
  draw_road(drawPos[0], drawPos[1], COLOR_YELLOW); /* draw */
}



short drawPos_one[2] = {screenWidth/2 +60,10}, controlPos_one[2] = {screenWidth/2 +59, 10};
short drawPos_two[2] = {screenWidth/2 +1,30}, controlPos_two[2] = {screenWidth/2, 30};


short colVelocity = -10, colLimits[2] = {-50, screenWidth};

void
draw_ball(int col, int row, unsigned short color)
{
    //draw each vertical rectangle
    for(char i = 0; i < 30; i++){
        if (col+i >= 0){
            fillRectangle(col+i,row, 1, 7, color);
        }
    }
    for(char i = 0; i < 15; i++){
        if (col+5+i >= 0){
            fillRectangle(col+5+i,row-5, 1, 5, color);
        }
    }
    for(char i = 0; i < 5; i++){
        if (col+20+i >= 0){
            fillRectangle(col+20+i,row-2, 1, 2, color);
        }
    }
}

void
screen_update_ball(short drawPos[2], short controlPos[2])
{
  for (char axis = 0; axis < 2; axis ++)
    if (drawPos[axis] != controlPos[axis]) /* position changed? */
      goto redraw;
  return;            /* nothing to do */
 redraw:
  draw_ball(drawPos[0], drawPos[1], COLOR_GRAY); /* erase */
  for (char axis = 0; axis < 2; axis ++)
    drawPos[axis] = controlPos[axis];
  draw_ball(drawPos[0], drawPos[1], WHITE); /* draw */
}
  

void move_clouds() {
    

    P1OUT |= 64;  // Green LED on when CPU on
    count++;
    if (count >= 90) {  // Adjust frequency as needed
        
        short oldCol_one = controlPos_one[0];
        short newCol_one = oldCol_one + colVelocity;

        // check if the ball is completely off screen on the left
        if (newCol_one <= -30)
            controlPos_one[0] = screenWidth;
        else
            controlPos_one[0] = newCol_one;
        
        short oldCol_two = controlPos_two[0];
        short newCol_two = oldCol_two + colVelocity;

        if (newCol_two + 30 < 0)
            controlPos_two[0] = screenWidth;
        else
            controlPos_two[0] = newCol_two;
        
        short oldCol_road_one = controlPos_road_one[0];
        short newCol_road_one = oldCol_road_one + colRoadVelocity;
        
        if (newCol_road_one + 30 < 0) 
            controlPos_road_one[0] = screenWidth;
        else
            controlPos_road_one[0] = newCol_road_one;
        
        short oldCol_road_two = controlPos_road_two[0];
        short newCol_road_two = oldCol_road_two + colRoadVelocity;
            
        if (newCol_road_two + 30 < 0)
            controlPos_road_two[0] = screenWidth;
        else
            controlPos_road_two[0] = newCol_road_two;
        
        redrawScreen = 1;  // Mark the screen for redrawing
        count = 0;
    }
    P1OUT &= ~64;  // Turn off the LED
}


void floor_bar(){
    if (floor_done){
        fillRectangle(0,screenHeight/2+20,screenWidth,3,BLACK);
        fillRectangle(0,screenHeight/2+40,screenWidth,3,BLACK);
        floor_done = 0;
    }
}
