/** \file lcddemo.c
 *  \brief A simple demo that draws a string and square
 */

#include <libTimer.h>
#include "lcdutils.h"
#include "lcddraw.h"
void fillRectangle(u_char colMin, u_char rowMin, u_char width, u_char height, u_int colorBGR);
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
