#include <msp430.h>
#include <libTimer.h>
#include "lcdutils.h"
#include "lcddraw.h"

typedef enum{
    WAITING,
    CHANGETIME,
    PREGAME,
    GAME
} State;

extern State current_state;
// WARNING: LCD DISPLAY USES P1.0.  Do not touch!!!

#define LED BIT6        /* note that bit zero req'd for display */

#define SW1 1
#define SW2 2
#define SW3 4
#define SW4 8

#define SWITCHES 15
void update_time(char change_hour, char change_minutes);
void refresh_screen();
void refresh_1();
void refresh_2();
void refresh_3();
void refresh_4();
void update_vars();

void blink_change_clock();
void state_waiting();
void state_change_time();
void transition(State next_state);
void display_clock();
void countdown_numbers();
void game();
void state_pre_game();
void state_game();

void update_shape();
void move_clouds();

void floor_bar();
char jump_flag = 0;
void screen_update_character();
void character_jump();
void screen_update_enemy();

char draw_floor_during_jump = 0;

char blue = 31, green = 0, red = 31;
unsigned char step = 0;


short hour = 0;
short minutes = 0;



static char
switch_update_interrupt_sense()
{
  char p2val = P2IN;
  /* update switch interrupt to detect changes from current buttons */
  P2IES |= (p2val & SWITCHES);    /* if switch up, sense down */
  
  return p2val;
}

void
switch_init()            /* setup switch */
{
  P2REN |= SWITCHES;        /* enables resistors for switches */
  P2IE |= SWITCHES;        /* enable interrupts from switches */
  P2OUT |= SWITCHES;        /* pull-ups for switches */
  P2DIR &= ~SWITCHES;        /* set switches' bits for input */
}

int switches = 0;


char button1;
char button2;
char button3;
char button4;

char changeTime = 0;

void
switch_interrupt_handler()
{
  char p2val = switch_update_interrupt_sense();
  
    button1 = (p2val & SW1) ? 0 : 1;
    button2 = (p2val & SW2) ? 0 : 1;
    button3 = (p2val & SW3) ? 0 : 1;
    button4 = (p2val & SW4) ? 0 : 1;
    
    switch(current_state){
        case WAITING:
            if (button1 && button4)
                transition(CHANGETIME);
            else if (button1){
                clearScreen(WHITE);
                transition(PREGAME);
            }
            break;
        case CHANGETIME:
            if (button2){// make it such that if hour is above 10, refresh position 1 and 2, otherwise refresh only 1
                changeTime = 1;
                hour++;
                update_time(1,0);
            } else if (button3){
                changeTime = 1;
                minutes++;
                update_time(0,1);
            } else if (button1 && button4)
                transition(WAITING);
            break;
        case PREGAME:
            break;
        case GAME:
            if (button2){
                jump_flag = 1;
            }
            if (button4){
                clearScreen(BLACK);
                transition(WAITING);
            }
    }
}


short drawPos_enemy[2] = {90,screenHeight/2 +25}, controlPos_enemy[2] = {90,screenHeight/2 +25};
short colEnemyVelocity = -10;






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









// axis zero for col, axis 1 for row

short drawPos_one[2] = {screenWidth/2 +60,10}, controlPos_one[2] = {screenWidth/2 +59, 10};
short drawPos_two[2] = {screenWidth/2 +1,40}, controlPos_two[2] = {screenWidth/2, 40};


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
    //fillRectangle(col, row, 30, 7, color);
//       fillRectangle(col+5, row-5, 15, 5, color);
//       fillRectangle(col+20, row-2, 5, 2, color);
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
  

int redrawScreen = 0;
u_int controlFontColor = COLOR_GREEN;


void refresh_1(){
    short first_digit_h = hour/10;
    short second_digit_h = hour % 10;
    clock_number(0,1,1);
    clock_number(0,2,1);
    clock_number(first_digit_h,1,0);
    clock_number(second_digit_h,2,0);
}
void refresh_2(){
    clock_number(0,2,1);
    clock_number(hour,2,0);
}
void refresh_3(){

    short first_digit_m = minutes/10;
    short second_digit_m = minutes % 10;
    clock_number(0,3,1);
    clock_number(0,4,1);
    clock_number(first_digit_m,3,0);
    clock_number(second_digit_m,4,0);

}
void refresh_4(){
    clock_number(0,4,1);
    clock_number(minutes,4,0);
}



void wdt_c_handler()
{
    switch (current_state) {
        case WAITING:
            state_waiting();
            break;
        case CHANGETIME:
            state_change_time();
            break;
        case PREGAME:
            state_pre_game();
            break;
        case GAME:
            state_game();
            break;
    }
}
    
    
    
//  secCount ++;
//  if (secCount >= 25) {        /* 10/sec */
//
//    {                /* move ball */
//      short oldCol = controlPos[0];
//      short newCol = oldCol + colVelocity;
//      if (newCol <= colLimits[0] || newCol >= colLimits[1])
//    colVelocity = -colVelocity;
//      else
//    controlPos[0] = newCol;
//    }
//
//    {                /* update hourglass */
//      if (switches & SW3) green = (green + 1) % 64;
//      if (switches & SW2) blue = (blue + 2) % 32;
//      if (switches & SW1) red = (red - 3) % 32;
//      if (step <= 30)
//    step ++;
//      else
//    step = 0;
//      secCount = 0;
//    }
//    if (switches & SW4) return;
//    redrawScreen = 1;
//  }

  
//void update_shape();
void main()
{
  
  P1DIR |= LED;        /**< Green led on when CPU on */
  P1OUT |= LED;
  configureClocks();
  lcd_init();
  switch_init();
  
  enableWDTInterrupts();      /**< enable periodic interrupt */
  or_sr(0x8);                  /**< GIE (enable interrupts) */
  clearScreen(BLACK);
    update_time(0,0);
    clock_colon();
    for(;;) {
      while (!redrawScreen) { /**< Pause CPU if screen doesn't need updating */
        P1OUT &= ~BIT6;    /**< Green led off witHo CPU */
        or_sr(0x10);          /**< CPU OFF */
      }
      
    if(current_state = GAME)
        screen_update_ball(drawPos_one, controlPos_one);
        screen_update_ball(drawPos_two, controlPos_two);
        screen_update_road(drawPos_road_one, controlPos_road_one);
        screen_update_road(drawPos_road_two, controlPos_road_two);
        
        
        floor_bar();
        screen_update_enemy();
        screen_update_character();
        
        
    }
}
              



void
update_time(char change_hour, char change_minutes){
    short first_digit_h = hour/10;
    short second_digit_h = hour % 10;
    short first_digit_m = minutes/10;
    short second_digit_m = minutes % 10;
    if (minutes >= 60){
        minutes = 0;
        hour++;
        change_minutes = 1;
        change_hour = 1;
        
    }
    if (hour >= 24){
        hour = 0;
        minutes = 0;
        change_minutes = 1;
        change_hour = 1;
    }
    
    if (change_hour){
        if (hour >= 10 || hour == 0)
            refresh_1();
        else if (hour < 10){
            refresh_2();
        }
    }
    if (change_minutes){
        
        if (minutes >= 10 || minutes == 0)
            refresh_3();
        else if (minutes < 10){
            refresh_4();
            
        }
    }
    else if (!(change_hour | change_minutes)){
        clock_number(first_digit_h, 1,0);
        clock_number(second_digit_h, 2,0);
        clock_number(first_digit_m, 3,0);
        clock_number(second_digit_m, 4,0);
    }
}


void
screen_update_hourglass()
{
  static unsigned char row = screenHeight / 2, col = screenWidth / 2;
  static char lastStep = 0;
  
  if (step == 0 || (lastStep > step)) {
    clearScreen(COLOR_BLUE);
    lastStep = 0;
  } else {
    for (; lastStep <= step; lastStep++) {
      int startCol = col - lastStep;
      int endCol = col + lastStep;
      int width = 1 + endCol - startCol;
      
      // a color in this BGR encoding is BBBB BGGG GGGR RRRR
      unsigned int color = (blue << 11) | (green << 5) | red;
      
      fillRectangle(startCol, row+lastStep, width, 1, color);
      fillRectangle(startCol, row-lastStep, width, 1, color);
    }
  }
}


    
void
update_shape()
{
//  screen_update_ball(drawPos_1, controlPos_1);
//  screen_update_ball(drawPos_2, controlPos_2);
  //screen_update_hourglass();
}
   


void
__interrupt_vec(PORT2_VECTOR) Port_2(){
  if (P2IFG & SWITCHES) {          /* did a button cause this interrupt? */
    P2IFG &= ~SWITCHES;              /* clear pending sw interrupts */
    switch_interrupt_handler();    /* single handler for all switches */
  }
}

static int secCount = 0;
static int totalSeconds = 0;


State current_state = WAITING;
void state_waiting(){
    display_clock();
}
static int time_sec = 0;
static char screen_off = 1;

void state_change_time(){
    blink_change_clock();
}

void state_pre_game(){
    countdown_numbers();
}

void state_game(){
    game();
    move_clouds();
    if (jump_flag)
        character_jump();
    
    
}
void transition(State next_state){
    update_vars();
    current_state = next_state;
}
void display_clock(){
      secCount++;
      if (secCount >= 250){
          secCount = 0;
          totalSeconds++;
          if (totalSeconds >= 60){
              totalSeconds = 0;
              minutes++;
              update_time(0,1);
          }
      }
      if (minutes >= 60){
          hour++;
          update_time(1,0);
      }
}


void blink_change_clock(){
    // every second, take away what is on the screen
    // another second goes by, put it back
    secCount++;
    if(secCount >= 250){
        secCount = 0;
        refresh_1();
        refresh_3();
    }
}


void update_vars(){
    //
    secCount = 0;
    totalSeconds = 0;
    
    
}


short cc = (screenWidth >> 1)-10;
short cr = (screenHeight >> 1)-40;


// change to how to play
void countdown_numbers(){
    // blank screen count down 3 2 1 in middle
    // game then starts after 3 seconds of counting down
    secCount++;
    if (secCount==1)
        draw_three(cc,cr,BLACK,WHITE);
    if(secCount == 250){
        clearScreen(COLOR_GRAY);
        draw_two(cc,cr,BLACK,WHITE);
    }
    if(secCount == 500){
        clearScreen(COLOR_GRAY);
        draw_one(cc,cr,BLACK,WHITE);
    }
    if(secCount == 750){
        clearScreen(COLOR_GRAY);
        transition(GAME);
    }
}

void game(){
    
}












int BALL_WIDTH = 30;
void move_clouds() {
    static short count = 0;
    static short count2 = 0;

    P1OUT |= BIT6;  // Green LED on when CPU on
    count++;
    count2++;
    if (count2 >= 50){
        count2=0;
        short oldCol_enemy = controlPos_enemy[0];
        short newCol_enemy = oldCol_enemy + colEnemyVelocity;  // colVelocity should be negative

        // Check if the ball is completely off-screen on the left
        if (newCol_enemy + BALL_WIDTH < 0) {  // BALL_WIDTH is the width of the ball
            // If off-screen, reset its position to just off-screen on the right
            controlPos_enemy[0] = screenWidth;  // Start from the right side again
            // You can adjust the vertical position randomly or keep it static as needed
            // Example for random vertical position within limits:
            /*controlPos[1] = 50; */ // Adjust range according to screen height
        } else {
            // Otherwise, update the position as before
            controlPos_enemy[0] = newCol_enemy;
        }
        redrawScreen = 1;  // Mark the screen for redrawing
    }
    
    
    
    
    
    
    
    if (count >= 90) {  // Adjust frequency as needed
        
        short oldCol_one = controlPos_one[0];
        short newCol_one = oldCol_one + colVelocity;  // colVelocity should be negative

        // Check if the ball is completely off-screen on the left
        if (newCol_one <= -30) {  // BALL_WIDTH is the width of the ball
            // If off-screen, reset its position to just off-screen on the right
            controlPos_one[0] = screenWidth;  // Start from the right side again
            // You can adjust the vertical position randomly or keep it static as needed
            // Example for random vertical position within limits:
            /*controlPos[1] = 50; */ // Adjust range according to screen height
        } else {
            // Otherwise, update the position as before
            controlPos_one[0] = newCol_one;
        }
        
        short oldCol_two = controlPos_two[0];
        short newCol_two = oldCol_two + colVelocity;  // colVelocity should be negative

        // Check if the ball is completely off-screen on the left
        if (newCol_two + BALL_WIDTH < 0) {  // BALL_WIDTH is the width of the ball
            // If off-screen, reset its position to just off-screen on the right
            controlPos_two[0] = screenWidth;  // Start from the right side again
            // You can adjust the vertical position randomly or keep it static as needed
            // Example for random vertical position within limits:
            /*controlPos[1] = 50; */ // Adjust range according to screen height
        } else {
            // Otherwise, update the position as before
            controlPos_two[0] = newCol_two;
        }
        
        
        short oldCol_road_one = controlPos_road_one[0];
        short newCol_road_one = oldCol_road_one + colRoadVelocity;  // colVelocity should be negative

        // Check if the ball is completely off-screen on the left
        if (newCol_road_one + BALL_WIDTH < 0) {  // BALL_WIDTH is the width of the ball
            // If off-screen, reset its position to just off-screen on the right
            controlPos_road_one[0] = screenWidth;  // Start from the right side again
            // You can adjust the vertical position randomly or keep it static as needed
            // Example for random vertical position within limits:
            /*controlPos[1] = 50; */ // Adjust range according to screen height
        } else {
            // Otherwise, update the position as before
            controlPos_road_one[0] = newCol_road_one;
        }
        
        short oldCol_road_two = controlPos_road_two[0];
        short newCol_road_two = oldCol_road_two + colRoadVelocity;  // colVelocity should be negative

        // Check if the ball is completely off-screen on the left
        if (newCol_road_two + BALL_WIDTH < 0) {  // BALL_WIDTH is the width of the ball
            // If off-screen, reset its position to just off-screen on the right
            controlPos_road_two[0] = screenWidth;  // Start from the right side again
            // You can adjust the vertical position randomly or keep it static as needed
            // Example for random vertical position within limits:
            /*controlPos[1] = 50; */ // Adjust range according to screen height
        } else {
            // Otherwise, update the position as before
            controlPos_road_two[0] = newCol_road_two;
        }
    
        redrawScreen = 1;  // Mark the screen for redrawing
        count = 0;
        
        
    }
    P1OUT &= ~BIT6;  // Turn off the LED
}


char floor_done = 1;
char floor_done2 = 1;
char floor_up = 1;
void floor_bar(){
    if (floor_done){
        fillRectangle(0,screenHeight/2+20,screenWidth,3,BLACK);
        fillRectangle(0,screenHeight/2+40,screenWidth,3,BLACK);
        floor_done = 0;
    }
}








short drawPosCharacter[2] = {10,screenHeight/2 +25};
short controlPosCharacter[2] = {10,screenHeight/2 +24};


short colVelocityCharacter = 10;
short colLimitsCharacter[2] = {(screenHeight/2) +25 -50, (screenHeight/2) +25};

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
    
    static int secCount = 0;
    static int secCountTwo = 0;
     secCount ++;
     if (secCount >= 25) {        /* 10/sec */
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
        redrawScreen = 1;  // Mark the screen for redrawing
        secCount = 0;
    }
    
}

//2
//2
//2


void
draw_enemy(int col, int row, unsigned short color1, unsigned short color2)
{
    
    
    for(char i = 0; i < 7; i++){
        if (col+i >= 0){
            fillRectangle(col+i, row-8, 1, 3,color1);
        }
    }
    for(char i = 0; i < 7; i++){
        if (col+i >= 0){
            fillRectangle(col+i,row-5, 1, 3, color2);
        }
    }
    for(char i = 0; i < 7; i++){
        if (col+i >= 0){
            fillRectangle(col+i,row-2, 1, 2, color1);
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
screen_update_enemy()
{
  for (char axis = 0; axis < 2; axis ++)
    if (drawPos_enemy[axis] != controlPos_enemy[axis]) /* position changed? */
      goto redraw;
  return;            /* nothing to do */
 redraw:
  draw_enemy(drawPos_enemy[0], drawPos_enemy[1], COLOR_GRAY, COLOR_BLACK); /* erase */
  for (char axis = 0; axis < 2; axis ++)
    drawPos_enemy[axis] = controlPos_enemy[axis];
  draw_enemy(drawPos_enemy[0], drawPos_enemy[1], COLOR_GREEN, COLOR_GREEN); /* draw */
}
