/** \file lcddraw.c
 *  \brief Adapted from RobG's EduKit
 */
#include "lcdutils.h"
#include "lcddraw.h"


/** Draw single pixel at x,row 
 *
 *  \param col Column to draw to
 *  \param row Row to draw to
 *  \param colorBGR Color of pixel in BGR
 */
void drawPixel(u_char col, u_char row, u_int colorBGR) 
{
  lcd_setArea(col, row, col, row);
  lcd_writeColor(colorBGR);
}

/** Fill rectangle
 *
 *  \param colMin Column start
 *  \param rowMin Row start
 *  \param width Width of rectangle
 *  \param height height of rectangle
 *  \param colorBGR Color of rectangle in BGR
 */
void fillRectangle(u_char colMin, u_char rowMin, u_char width, u_char height, 
		   u_int colorBGR)
{
  u_char colLimit = colMin + width, rowLimit = rowMin + height;
  lcd_setArea(colMin, rowMin, colLimit - 1, rowLimit - 1);
  u_int total = width * height;
  u_int c = 0;
  while ((c++) < total) {
    lcd_writeColor(colorBGR);
  }
}

/** Clear screen (fill with color)
 *  
 *  \param colorBGR The color to fill screen
 */
void clearScreen(u_int colorBGR) 
{
  u_char w = screenWidth;
  u_char h = screenHeight;
  fillRectangle(0, 0, screenWidth, screenHeight, colorBGR);
}

/** 5x7 font - this function draws background pixels
 *  Adapted from RobG's EduKit
 */
void drawChar5x7(u_char rcol, u_char rrow, char c,
     u_int fgColorBGR, u_int bgColorBGR)
{
  u_char col = 0;
  u_char row = 0;
  u_char bit = 0x01;
  u_char oc = c - 0x20;

  lcd_setArea(rcol, rrow, rcol + 4, rrow + 7); /* relative to requested col/row */
  while (row < 8) {
    while (col < 5) {
      u_int colorBGR = (font_5x7[oc][col] & bit) ? fgColorBGR : bgColorBGR;
      lcd_writeColor(colorBGR);
      col++;
    }
    col = 0;
    bit <<= 1;
    row++;
  }
}


void drawChar11x16(u_char rcol, u_char rrow, char c,
     u_int fgColorBGR, u_int bgColorBGR)
{
  u_char col = 0;
  u_char row = 0;
  unsigned short bit = 0x0001;
  u_char oc = c - 0x20;

  lcd_setArea(rcol, rrow, rcol + 10, rrow + 16); /* relative to requested col/row */
  while (row < 17) {
    while (col < 11) {
      u_int colorBGR = (font_11x16[oc][col] & bit) ? fgColorBGR : bgColorBGR;
      lcd_writeColor(colorBGR);
      col++;
    }
    col = 0;
    bit <<= 1;
    row++;
  }
}








/** Draw string at col,row
 *  Type:
 *  FONT_SM - small (5x8,) FONT_MD - medium (8x12,) FONT_LG - large (11x16)
 *  FONT_SM_BKG, FONT_MD_BKG, FONT_LG_BKG - as above, but with background color
 *  Adapted from RobG's EduKit
 *
 *  \param col Column to start drawing string
 *  \param row Row to start drawing string
 *  \param string The string
 *  \param fgColorBGR Foreground color in BGR
 *  \param bgColorBGR Background color in BGR
 */



void drawString5x7(u_char col, u_char row, char *string,
		u_int fgColorBGR, u_int bgColorBGR)
{
  u_char cols = col;
  while (*string) {
    drawChar5x7(cols, row, *string++, fgColorBGR, bgColorBGR);
    cols += 6;
  }
}


/** Draw rectangle outline
 *  
 *  \param colMin Column start
 *  \param rowMin Row start 
 *  \param width Width of rectangle
 *  \param height Height of rectangle
 *  \param colorBGR Color of rectangle in BGR
 */
void drawRectOutline(u_char colMin, u_char rowMin, u_char width, u_char height,
             u_int colorBGR)
{
  /**< top & bot */
  fillRectangle(colMin, rowMin, width, 1, colorBGR);
  fillRectangle(colMin, rowMin + height, width, 1, colorBGR);

  /**< left & right */
  fillRectangle(colMin, rowMin, 1, height, colorBGR);
  fillRectangle(colMin + width, rowMin, 1, height, colorBGR);
}


short widthCenter = screenWidth >> 1;
short heightCenter = screenHeight >> 1;

void clock_number(u_char digit, u_char pos){
    // just switch off of position then switch digit
    short col;
    short row;
    switch(pos){
        case 1:
            col = (widthCenter - (screenWidth/2))+10;
            row = screenHeight/4;
            break;
        case 2:
            col = widthCenter - (screenWidth/4)+3;
            row = screenHeight/4;
            break;
        case 3:
            col = widthCenter + (screenWidth/4)-20 -3;
            row = screenHeight/4;
            break;
        case 4:
            col = (widthCenter + (screenWidth/2)-20)-10;
            row = screenHeight/4;
            break;
    }
    switch(digit){
        case 0:
            draw_zero(col, row);
            break;
        case 1:
            draw_one(col, row);
            break;
        case 2:
            draw_two(col, row);
            break;
        case 3:
            draw_three(col, row);
            break;
        case 4:
            draw_four(col, row);
            break;
        case 5:
            draw_five(col, row);
            break;
        case 6:
            draw_six(col, row);
            break;
        case 7:
            draw_seven(col, row);
            break;
        case 8:
            draw_eight(col, row);
            break;
        case 9:
            draw_nine(col, row);
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
