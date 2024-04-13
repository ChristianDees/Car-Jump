/** \file lcddemo.c
 *  \brief A simple demo that draws a string and square
 */

#include <libTimer.h>
#include "lcdutils.h"
#include "lcddraw.h"
void fillRectangle(u_char colMin, u_char rowMin, u_char width, u_char height, u_int colorBGR);
void clock_number(u_char digit, u_char pos);
//void drawRectOutline(u_char colMin, u_char rowMin, u_char width, u_char height,
                     //u_int colorBGR);
void draw_zero(short col, short row);
void draw_one(short col, short row);
void draw_two(short col, short row);
void draw_three(short col, short row);
void draw_four(short col, short row);
void draw_five(short col, short row);
void draw_six(short col, short row);
void draw_seven(short col, short row);
void draw_eight(short col, short row);
void draw_nine(short col, short row);
void clock_colon();
/** Initializes everything, clears the screen, draws "hello" and a square */
int
main()
{
  configureClocks();
  lcd_init();
  u_char width = screenWidth, height = screenHeight;

  clearScreen(COLOR_BLUE);
  //drawPixel(screenWidth>>1, screenHeight>>1, BLACK);
  
    
    clock_colon();
    clock_number(0, 1);
    clock_number(9, 2);
    clock_number(0, 3);
    clock_number(0, 4);
    

    
    
    
    
    
    
  // drawString5x7(20,20, "hello", COLOR_GREEN, COLOR_RED);

  //fillRectangle(30,30, 60, 60, COLOR_ORANGE);
    
  short centerCol = screenWidth >> 1;
  short centerRow = screenHeight >> 1;
    
//    int x = 0;
//    int y = 0;
//    int speed = 6;
    
    
//  for (short i = 0; i < screenWidth; i++){
//      drawPixel(x, centerRow - y, COLOR_PINK);
//      drawPixel(x, centerRow - speed, COLOR_PINK);
//      x++;
//      y+=speed;
//      speed-=1;
//      if (y<=0)
//          speed = -speed;
//  }

    // start in middle.
    // increase rows to go down
    // each time increase row, increase column to add one
    int x = centerCol;
    int y = centerRow;
    int size = 50;
    
    

    
    // top right facing traingle
    /*
    int startCol = centerCol - size / 2;
    int startRow = centerRow - size / 2;
    for (short i = 1; i <= size; ++i) {
        for (short j = 1; j <= i; ++j) {
            drawPixel(startCol + i, startRow + j, BLACK);
        }
    }
    */
    
    // bottom right facing triangle
    /*
    int startCol = centerCol - size / 2;
    int startRow = centerRow + size / 2;
    for (short i = 1; i <= size; ++i) {
        for (short j = 1; j <= i; ++j) {
            drawPixel(startCol + i, startRow - j, BLACK);
        }
    }
    */
    
    /*
    // bottom left facing triangle
    int startCol = centerCol + size / 2;
    int startRow = centerRow + size / 2;
    for (short i = 1; i <= size; ++i) {
        for (short j = 1; j <= i; ++j) {
            drawPixel(startCol - i, startRow - j, BLACK);
        }
    }
    */
    
    
    // top left facing triangle
    /*
    int startCol = centerCol - size / 2;
    int startRow = centerRow - size / 2;
    for (short i = 1; i <= size; ++i) {
        for (short j = 1; j <= i; ++j) {
            drawPixel(startCol + size - i, startRow + j, BLACK);
        }
    }
    */
    
    // acute triangle downwards
    /*
    int startCol = centerCol - size / 2;
    int startRow = centerRow + size / 2;

    for (short i = 0; i < size; ++i) {
        for (short j = 0; j < 2 * i + 1; ++j) {
            drawPixel(startCol + size / 2 - i + j, startRow - i, BLACK);
        }
    }
    */
    
    // acute triangle upwards
    /*
    int startCol = centerCol - size / 2;
    int startRow = centerRow - size / 2;

    for (short i = 0; i < size; ++i) {
        for (short j = 0; j < 2 * i + 1; ++j) {
            drawPixel(startCol + size / 2 - i + j, startRow + i, BLACK);
        }
    }
    */
    
    
    // diamond shape
//    int startCol = centerCol - size / 2;
//    int startRow = centerRow - size;
//
//    for (short i = 0; i < size; ++i) {
//        for (short j = 0; j < 2 * i + 1; ++j) {
//            drawPixel(startCol + size / 2 - i + j, startRow + i, BLACK);
//        }
//    }
//    startCol = centerCol - size / 2;
//    startRow = centerRow + size - 1;
//    for (short i = 0; i < size; ++i) {
//        for (short j = 0; j < 2 * i + 1; ++j) {
//            drawPixel(startCol + size / 2 - i + j, startRow - i, BLACK);
//        }
//    }
    
    

    
    
}
// draw_clock_number(20, width/4, 4, width-(width/4), BLACK);
//                   NO, YES
// add parameters for 1st digit, 2nd digit, 3rd digit, 4th digit
short widthCenter = screenWidth >> 1;
short heightCenter = screenHeight >> 1;

void clock_number(u_char digit, u_char pos){
    // just switch off of position then switch digit
    
    
    
    switch(digit){
        case 0:
            if (pos == 1){
                
                
                short col = (widthCenter - (screenWidth/2))+10;
                short row = screenHeight/4;
                draw_zero(col, row);
        

                    
            } else if (pos == 2){
                
                short col = widthCenter - (screenWidth/4)+3;
                short row = screenHeight/4;
                draw_zero(col, row);
                
            } else if (pos == 3){
                
                short col = widthCenter + (screenWidth/4)-20 -3;
                short row = screenHeight/4;
                draw_zero(col, row);
                
            } else if (pos == 4){
                
                short col = (widthCenter + (screenWidth/2)-20)-10;
                short row = screenHeight/4;
                draw_zero(col, row);
                
            }
            break;
        case 1:
            if (pos == 1){
                short col = (widthCenter - (screenWidth/2))+10;
                short row = screenHeight/4;
                draw_one(col, row);
                
            } else if (pos == 2){
                short col = widthCenter - (screenWidth/4)+3;
                short row = screenHeight/4;
                draw_one(col, row);
                
            } else if (pos == 3){
                short col = widthCenter + (screenWidth/4)-20 -3;
                short row = screenHeight/4;
                draw_one(col, row);

            } else if (pos == 4){
                short col = (widthCenter + (screenWidth/2)-20)-10;
                short row = screenHeight/4;
                draw_one(col, row);
                
            }
            break;
        case 2:
            if (pos == 1){
                short col = (widthCenter - (screenWidth/2))+10;
                short row = screenHeight/4;
                draw_two(col, row);
            } else if (pos == 2){
                short col = widthCenter - (screenWidth/4)+3;
                short row = screenHeight/4;
                draw_two(col, row);
            } else if (pos == 3){
                short col = widthCenter + (screenWidth/4)-20 -3;
                short row = screenHeight/4;
                draw_two(col, row);
            } else if (pos == 4){
                short col = (widthCenter + (screenWidth/2)-20)-10;
                short row = screenHeight/4;
                draw_two(col, row);
            }
            break;
        case 3:
            if (pos == 1){
                short col = (widthCenter - (screenWidth/2))+10;
                short row = screenHeight/4;
                draw_three(col, row);
            } else if (pos == 2){
                short col = widthCenter - (screenWidth/4)+3;
                short row = screenHeight/4;
                draw_three(col, row);
            } else if (pos == 3){
                short col = widthCenter + (screenWidth/4)-20 -3;
                short row = screenHeight/4;
                draw_three(col, row);
            } else if (pos == 4){
                short col = (widthCenter + (screenWidth/2)-20)-10;
                short row = screenHeight/4;
                draw_three(col, row);
            }
            break;
        case 4:
            if (pos == 1){
                short col = (widthCenter - (screenWidth/2))+10;
                short row = screenHeight/4;
                draw_four(col, row);
            } else if (pos == 2){
                short col = widthCenter - (screenWidth/4)+3;
                short row = screenHeight/4;
                draw_four(col, row);
            } else if (pos == 3){
                short col = widthCenter + (screenWidth/4)-20 -3;
                short row = screenHeight/4;
                draw_four(col, row);
            } else if (pos == 4){
                short col = (widthCenter + (screenWidth/2)-20)-10;
                short row = screenHeight/4;
                draw_four(col, row);
            }
            break;
        case 5:
            if (pos == 1){
                short col = (widthCenter - (screenWidth/2))+10;
                short row = screenHeight/4;
                draw_five(col, row);
            } else if (pos == 2){
                short col = widthCenter - (screenWidth/4)+3;
                short row = screenHeight/4;
                draw_five(col, row);
            } else if (pos == 3){
                short col = widthCenter + (screenWidth/4)-20 -3;
                short row = screenHeight/4;
                draw_five(col, row);
            } else if (pos == 4){
                short col = (widthCenter + (screenWidth/2)-20)-10;
                short row = screenHeight/4;
                draw_five(col, row);
            }
            break;
        case 6:
            if (pos == 1){
                short col = (widthCenter - (screenWidth/2))+10;
                short row = screenHeight/4;
                draw_six(col, row);
            } else if (pos == 2){
                short col = widthCenter - (screenWidth/4)+3;
                short row = screenHeight/4;
                draw_six(col, row);
            } else if (pos == 3){
                short col = widthCenter + (screenWidth/4)-20 -3;
                short row = screenHeight/4;
                draw_six(col, row);
            } else if (pos == 4){
                short col = (widthCenter + (screenWidth/2)-20)-10;
                short row = screenHeight/4;
                draw_six(col, row);
            }
            break;
        case 7:
            if (pos == 1){
                short col = (widthCenter - (screenWidth/2))+10;
                short row = screenHeight/4;
                draw_seven(col, row);
            } else if (pos == 2){
                short col = widthCenter - (screenWidth/4)+3;
                short row = screenHeight/4;
                draw_seven(col, row);
            } else if (pos == 3){
                short col = widthCenter + (screenWidth/4)-20 -3;
                short row = screenHeight/4;
                draw_seven(col, row);
            } else if (pos == 4){
                short col = (widthCenter + (screenWidth/2)-20)-10;
                short row = screenHeight/4;
                draw_seven(col, row);
            }
            break;
        case 8:
            if (pos == 1){
                short col = (widthCenter - (screenWidth/2))+10;
                short row = screenHeight/4;
                draw_eight(col, row);
            } else if (pos == 2){
                short col = widthCenter - (screenWidth/4)+3;
                short row = screenHeight/4;
                draw_eight(col, row);
            } else if (pos == 3){
                short col = widthCenter + (screenWidth/4)-20 -3;
                short row = screenHeight/4;
                draw_eight(col, row);
            } else if (pos == 4){
                short col = (widthCenter + (screenWidth/2)-20)-10;
                short row = screenHeight/4;
                draw_eight(col, row);
            }
            break;
            break;
        case 9:
            if (pos == 1){
                short col = (widthCenter - (screenWidth/2))+10;
                short row = screenHeight/4;
                draw_nine(col, row);
            } else if (pos == 2){
                short col = widthCenter - (screenWidth/4)+3;
                short row = screenHeight/4;
                draw_nine(col, row);
            } else if (pos == 3){
                short col = widthCenter + (screenWidth/4)-20 -3;
                short row = screenHeight/4;
                draw_nine(col, row);
            } else if (pos == 4){
                short col = (widthCenter + (screenWidth/2)-20)-10;
                short row = screenHeight/4;
                draw_nine(col, row);
            }
            break;
        
            
    }
}



void clock_colon(){
    short centerCol = screenWidth >> 1;
    short centerRow = screenHeight >> 1;
    fillRectangle(centerCol-3, centerRow-27, 6, 6, BLACK);
    fillRectangle(centerCol-3, centerRow+27, 6, 6,BLACK);
}


void draw_zero(short col, short row){

    fillRectangle(col, row, 20, screenWidth-(row), BLACK);
    fillRectangle(col+5, row+5, 10, screenWidth-(row)-10, COLOR_BLUE);
}


void draw_one(short col, short row){
    fillRectangle(col, row, 10, 5, BLACK);
    fillRectangle(col+8, row, 5,screenWidth-(row), BLACK);
    fillRectangle(col,row+(screenWidth-(row))-5, 20, 5, BLACK);
}

void draw_two(short col, short row){
    short depth = screenWidth-row;
    short depth_vertical = depth/2;
    fillRectangle(col, row, 20, 5, BLACK); // top bar
    fillRectangle(col+15, row, 5,depth_vertical, BLACK); // right vertical
    fillRectangle(col, row + depth_vertical, 20, 5, BLACK); // second bar
    fillRectangle(col, row + depth_vertical, 5, depth_vertical, BLACK); // left vertical
    fillRectangle(col, row + depth - 5, 20, 5, BLACK); // bottom bar
}


void draw_three(short col, short row){
    short depth = screenWidth-row;
    short depth_vertical = depth/2;
    fillRectangle(col, row, 20, 5, BLACK); // top bar
    fillRectangle(col+15, row, 5,depth, BLACK); // right vertical
    fillRectangle(col, row + depth_vertical, 20, 5, BLACK); // second bar
    fillRectangle(col, row + depth - 5, 20, 5, BLACK); // bottom bar
}

void draw_four(short col, short row){
    short depth = screenWidth-row;
    short depth_vertical = depth/2;
    fillRectangle(col, row, 5,depth_vertical, BLACK); // left vertical
    fillRectangle(col, row + depth_vertical, 20, 5, BLACK); // middle bar
    fillRectangle(col+15, row, 5,depth, BLACK); // right vertical
}

void draw_five(short col, short row){
    short depth = screenWidth-row;
    short depth_vertical = depth/2;
    fillRectangle(col, row, 20, 5, BLACK); // top bar
    fillRectangle(col, row, 5, depth_vertical, BLACK); // left vertical
    fillRectangle(col, row + depth_vertical, 20, 5, BLACK); // second bar
    fillRectangle(col+15, row + depth_vertical, 5,depth_vertical, BLACK); // right vertical
    fillRectangle(col, row + depth - 5, 20, 5, BLACK); // bottom bar
}



/*
 
 ****
 *   *
    *
    *
    *
    *

 */
void draw_six(short col, short row){
    short depth = screenWidth-row;
    short depth_vertical = depth/2;
    fillRectangle(col, row, 20, 5, BLACK); // top bar
    fillRectangle(col, row, 5, depth, BLACK); // left vertical
    fillRectangle(col, row + depth_vertical, 20, 5, BLACK); // second bar
    fillRectangle(col+15, row + depth_vertical, 5,depth_vertical, BLACK); // right vertical
    fillRectangle(col, row + depth - 5, 20, 5, BLACK); // bottom bar
}

void draw_seven(short col, short row){
    fillRectangle(col, row, 20, 5, BLACK);
    fillRectangle(col+15, row, 5,screenWidth-(row), BLACK);
}

void draw_eight(short col, short row){
    fillRectangle(col, row, 20, screenWidth-(row), BLACK); // black rectangle
    fillRectangle(col+5, row+5, 10, screenWidth-(row)-10, COLOR_BLUE); // hallow inside
    fillRectangle(col, row + (screenWidth-row)/2, 20, 5, BLACK); // second bar
}

void draw_nine(short col, short row){
    short depth = screenWidth-row;
    short depth_vertical = depth/2;
    fillRectangle(col, row, 20, 5, BLACK); // top bar
    fillRectangle(col, row, 5,depth_vertical, BLACK); // left vertical
    fillRectangle(col, row + depth_vertical, 20, 5, BLACK); // middle bar
    fillRectangle(col+15, row, 5,depth, BLACK); // right vertical
    fillRectangle(col, row + depth - 5, 20, 5, BLACK); // bottom bar
    
}
