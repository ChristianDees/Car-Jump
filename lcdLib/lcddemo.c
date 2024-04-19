/** \file lcddemo.c
 *  \brief A simple demo that draws a string and square
 */

#include <libTimer.h>
#include "lcdutils.h"
#include "lcddraw.h"
void fillRectangle(u_char colMin, u_char rowMin, u_char width, u_char height, u_int colorBGR);
void draw_car(short col, short row);
void draw_car2(short col, short row);

/** Initializes everything, clears the screen, draws "hello" and a square */
int
main()
{
  configureClocks();
  lcd_init();
  u_char width = screenWidth, height = screenHeight;

  clearScreen(WHITE);
  //drawPixel(screenWidth>>1, screenHeight>>1, BLACK);
    short centerCol = screenWidth >> 1;
    short centerRow = screenHeight >> 1;
    
    //draw_car2(centerCol+10, centerRow-40);
    
    int size = 40;
    int startCol = centerCol - size / 2; // Start from the rightmost point
    int startRow = centerRow - size / 2;
    
    
    fillRectangle(startCol, startRow, size, 3, BLACK);
    fillRectangle(startCol, startRow + size-3, size, 3, BLACK);

    /**< left & right */
    fillRectangle(startCol, startRow, 3, size, BLACK);
    fillRectangle(startCol + size-3, startRow, 3, size, BLACK);
    
    
    
    
//    drawRectOutline(startCol, startRow, size, size, BLACK);
    fillRectangle(startCol+3, startRow+3, size-6, size-6, WHITE);
    fillRectangle(centerCol-6, startRow+8, 5, 24, BLACK);
    fillRectangle(centerCol+1, startRow+8, 5, 24, BLACK);


//    fillRectangle(startCol-10, startRow, 5, 10, WHITE);
//    fillRectangle(startCol+10, startRow-20, 5, 10, WHITE);

    
    
    
    
//    for (short i = 0; i < size; ++i) {
//        for (short j = 0; j < 2 * i + 1; ++j) {
//            drawPixel(startCol - i, startRow + size / 2 - i + j, BLACK);
//        }
//    }

    
    
    
    
    
    
  // drawString5x7(20,20, "hello", COLOR_GREEN, COLOR_RED);

  //fillRectangle(30,30, 60, 60, COLOR_ORANGE);
    
  
    
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
void draw_cloud(short col, short row){
    fillRectangle(col, row, 30, 7, COLOR_GRAY);
    fillRectangle(col+5, row-5, 15, 5, COLOR_GRAY);
    fillRectangle(col+20, row-2, 5, 2, COLOR_GRAY);
}

void draw_car(short col, short row){
    fillRectangle(col+3, row, 4,2, COLOR_BLACK); //wheel left
    fillRectangle(col+12, row, 4,2, COLOR_BLACK); //wheel right
    fillRectangle(col, row-6, 20, 6, COLOR_BLUE); //bottom of car
    fillRectangle(col+4, row-11, 10, 5, COLOR_BLUE); // top of car
    fillRectangle(col+7, row-9, 5, 4, WHITE); // top of car
}

void draw_car2(short col, short row){
    fillRectangle(col+3, row, 4,2, BLACK); //wheel left
    fillRectangle(col+12, row, 4,2, BLACK); //wheel right
    
    fillRectangle(col, row-1, 20, 1,COLOR_GREEN);
    
    fillRectangle(col, row-3, 20, 1,BLACK);
    
    fillRectangle(col, row-4, 20, 2,COLOR_BLUE);
    
    fillRectangle(col, row-6, 20, 2,COLOR_YELLOW);

    
    fillRectangle(col+4, row-11, 10, 5, COLOR_BLUE); // top of car
    fillRectangle(col+7, row-9, 5, 3, COLOR_WHITE); // top of car
}
