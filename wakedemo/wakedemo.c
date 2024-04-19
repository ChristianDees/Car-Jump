#include <msp430.h>
#include <libTimer.h>
#include <stdio.h>
#include "lcdutils.h"
#include "lcddraw.h"


typedef enum{
    WAITING,
    CHANGETIME,
    CONTROLPAGEONE,
    CONTROLPAGETWO,
    GAME,
    PAUSE,
    GAMEOVER
} State;

extern State current_state;
// WARNING: LCD DISPLAY USES P1.0.  Do not touch!!!

#define LED BIT6        /* note that bit zero req'd for display */

#define SW1 1
#define SW2 2
#define SW3 4
#define SW4 8

#define SWITCHES 15
void int_to_string_simple(int n, char *str, size_t size);
void update_time(char change_hour, char change_minutes);
void refresh_screen();
void refresh_1();
void refresh_2();
void refresh_3();
void refresh_4();
void update_vars();
void display_score();
void blink_change_clock();
void state_waiting();
void state_change_time();
void transition(State next_state);
void display_clock();
void game();
void state_control_page_one();
void state_control_page_two();

void state_game();
char pause_flag = 0;
void update_shape();
void move_clouds();
void screen_update_score();
void floor_bar();
char jump_flag = 0;
void screen_update_character();
void character_jump();
void screen_update_enemy(u_int colorBGR);
void screen_update_enemy_two(u_int colorBGR);
char enable_second_enemy = 0;
void state_game_over();
char do_overlap(char cTLX, char cTLY, char cBRX, char cBRY, char eTLX, char eTLY, char eBRX, char eBRY);
char floor_done = 1;
void state_pause();
void display_pause();
void display_controls();
void display_controls_two();
int high_score = 0;
int current_score = 0;
char blue = 31, green = 0, red = 31;
unsigned char step = 0;
char display_pause_once = 1;
char reset_pause_flag= 0;
void reset_pause();
short hour = 0;
short minutes = 0;
char overlap_flag = 0;
char seconds = 0;
char display_controls_once = 1;























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
            if (button4)
                transition(CHANGETIME);
            else if (button1){
                clearScreen(COLOR_GRAY);
                transition(CONTROLPAGEONE);
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
            } else if (button1){
                transition(WAITING);
            }
            break;
        case CONTROLPAGEONE:
            if(button1){
                clearScreen(COLOR_GRAY);
                transition(GAME);
            }
            if(button3){
                clearScreen(BLACK);
                transition(WAITING);
            }
            if(button4){
                clearScreen(COLOR_GRAY);
                
                transition(CONTROLPAGETWO);
            }
            break;
        case CONTROLPAGETWO:
            if(button3){
                clearScreen(COLOR_GRAY);
                
                transition(CONTROLPAGEONE);
            }
            break;
        case GAME:
            
            if (button2){
                jump_flag = 1;
            }
            
            if (button3){
                pause_flag = 1;
            }
            break;
        case PAUSE:
            if (button3){
                reset_pause_flag = 1;
            }
            if (button4){
                clearScreen(BLACK);
                transition(WAITING);
            }
            break;
        case GAMEOVER:
            if (button1){
                clearScreen(COLOR_GRAY);
                transition(GAME);
            }
            if (button4){
                clearScreen(BLACK);
                transition(WAITING);
            }
            break;
            
        
    }
}










short drawPos_enemy[2] = {60,screenHeight/2 +24}, controlPos_enemy[2] = {61,screenHeight/2 +24};
short colEnemyVelocity = -10;

short drawPos_enemy_two[2] = {130,screenHeight/2 +24}, controlPos_enemy_two[2] = {131,screenHeight/2 +24};
short colEnemyVelocity_two = -10;





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



short drawPosCharacter[2] = {10,screenHeight/2 +25};
short controlPosCharacter[2] = {10,screenHeight/2 +24};


short colVelocityCharacter = 12;
short colLimitsCharacter[2] = {(screenHeight/2) +25 -60, (screenHeight/2) +25};






// axis zero for col, axis 1 for row

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
        case CONTROLPAGEONE:
            state_control_page_one();
            break;
        case CONTROLPAGETWO:
            state_control_page_two();
            break;
        case GAME:
            state_game();
            break;
        case PAUSE:
            state_pause();
            break;
        case GAMEOVER:
            state_game_over();
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
u_int colorBGR;
u_int colorBGR_two;
  
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
            
            
            if(overlap_flag){
                transition(GAMEOVER);
            }
            
            if (pause_flag){

                pause_flag = 0;
                current_state = PAUSE;
            }
            
        }
//        if (current_state == PAUSE){
//            
//            if (reset_pause_flag){
//                
//                current_state = GAME;
//                redrawScreen = 1;
//                //reset_pause_flag = 0;
//            }
//            
//        }
        
        redrawScreen = 0;
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

void state_control_page_one(){
    display_controls();
}
void state_control_page_two(){
    display_controls_two();
}

void state_game(){
    game();
    move_clouds();
    if (jump_flag)
        character_jump();
    display_score();
    
    
}
void state_pause(){
    display_pause();
    reset_pause();
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
    if (!reset_pause_flag && current_state == PAUSE)
        redrawScreen = 0;
}




char score_once = 1;
char seconds_score = 0;
void display_score(){
    if (score_once){
        char str[20]; // Adjust the size as needed
        int_to_string_simple(high_score, str, sizeof(str));
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






static char draw_once = 1;
void state_game_over(){
    if (draw_once){
        fillRectangle(30, screenWidth/2 -15, 65, 45, COLOR_BLACK);
        drawString11x16(35,screenWidth/2 -10, "Game", COLOR_RED, BLACK);
        drawString11x16(35,screenWidth/2 +10, "Over", COLOR_RED, BLACK);
        draw_once = 0;
        redrawScreen = 1;
    }
    redrawScreen = 0;
    
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

static short count = 0;
static short count2 = 0;
void update_vars(){
    //
    
    int x = ((high_score + drawPosCharacter[1]) % 4) +1;
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
    
    
    colEnemyVelocity = -10;
    drawPos_enemy[0] = 60;
    drawPos_enemy[1] = screenHeight/2 +24;
    controlPos_enemy[0] = 61;
    controlPos_enemy[1] = screenHeight/2 +24;

    colEnemyVelocity_two = -10;
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
    
    
}


short cc = (screenWidth >> 1)-10;
short cr = (screenHeight >> 1)-40;


// change to how to play
void display_controls(){
    // blank screen count down 3 2 1 in middle
    // game then starts after 3 seconds of counting down
    if (display_controls_once){
        drawString5x7(5,5, "<- SW3", BLACK, COLOR_GRAY);
        drawString5x7(screenWidth-40,5, "SW4 ->", BLACK, COLOR_GRAY);
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
    redrawScreen = 0;
}

void display_controls_two(){
    // blank screen count down 3 2 1 in middle
    // game then starts after 3 seconds of counting down
    if (display_controls_once){
        drawString5x7(5,5, "<- SW3", BLACK, COLOR_GRAY);
        drawString11x16(40,20, "Time", BLACK, COLOR_GRAY);
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
    redrawScreen = 0;
}

void game(){
    
}












int BALL_WIDTH = 30;
void move_clouds() {
    

    P1OUT |= BIT6;  // Green LED on when CPU on
    count++;
    count2++;
    
    if (count2 >= 50){
        seconds++;
        count2=0;
        short oldCol_enemy = controlPos_enemy[0];
        short newCol_enemy = oldCol_enemy + colEnemyVelocity;  // colVelocity should be negative

        // Check if the ball is completely off-screen on the left
        if (newCol_enemy + BALL_WIDTH < 0) {  // BALL_WIDTH is the width of the ball
            // If off-screen, reset its position to just off-screen on the right
            int x;
            if (current_score > high_score)
                x = (current_score - high_score) + 1;
            else
                x = (high_score - current_score) + 1;
            x = x%4;
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
            if (newCol_enemy_two + BALL_WIDTH < 0) {  // BALL_WIDTH is the width of the ball
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




void floor_bar(){
    if (floor_done){
        fillRectangle(0,screenHeight/2+20,screenWidth,3,BLACK);
        fillRectangle(0,screenHeight/2+40,screenWidth,3,BLACK);
        floor_done = 0;
    }
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


void int_to_string_simple(int n, char *str, size_t size) {
    if (n == 0) {
        str[0] = '0';
        str[1] = '\0';
        return;
    }

    int i = 0;
    while (n != 0 && i < size - 1) {
        int digit = n % 10;
        str[i++] = '0' + digit;
        n /= 10;
    }
    str[i] = '\0';

    // Reverse the string
    int start = 0;
    int end = i - 1;
    while (start < end) {
        char temp = str[start];
        str[start] = str[end];
        str[end] = temp;
        start++;
        end--;
    }
}

// Usage









void screen_update_score(){
    if (current_score > high_score)
        high_score = current_score;
    char str[20]; // Adjust the size as needed
    int_to_string_simple(current_score, str, sizeof(str));
    drawString8x12(90, 135, str, BLACK, COLOR_GRAY);
    
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
         if (do_overlap(drawPosCharacter[0], drawPosCharacter[1]-11, drawPosCharacter[0]+16, drawPosCharacter[1],drawPos_enemy[0]+3, drawPos_enemy[1]-11, drawPos_enemy[0]+16, drawPos_enemy[1]))
             overlap_flag = 1;
         if (do_overlap(drawPosCharacter[0], drawPosCharacter[1]-11, drawPosCharacter[0]+16, drawPosCharacter[1],drawPos_enemy_two[0]+3, drawPos_enemy_two[1]-11, drawPos_enemy_two[0]+16, drawPos_enemy_two[1]))
             overlap_flag = 1;
         if (do_overlap(drawPosCharacter[0]-3, drawPosCharacter[1]-11, drawPosCharacter[0]+16, drawPosCharacter[1],drawPos_enemy[0], drawPos_enemy[1]-11, drawPos_enemy[0]+16, drawPos_enemy[1]))
             overlap_flag = 1;
         if (do_overlap(drawPosCharacter[0]-3, drawPosCharacter[1]-11, drawPosCharacter[0]+16, drawPosCharacter[1],drawPos_enemy_two[0], drawPos_enemy_two[1]-11, drawPos_enemy_two[0]+16, drawPos_enemy_two[1]))
             overlap_flag = 1;
         
        redrawScreen = 1;  // Mark the screen for redrawing
        
        secCount = 0;
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



