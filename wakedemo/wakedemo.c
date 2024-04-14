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

#define LED BIT6		/* note that bit zero req'd for display */

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


char blue = 31, green = 0, red = 31;
unsigned char step = 0;


short hour = 0;
short minutes = 0;

static char
switch_update_interrupt_sense()
{
  char p2val = P2IN;
  /* update switch interrupt to detect changes from current buttons */
  P2IES |= (p2val & SWITCHES);	/* if switch up, sense down */
  
  return p2val;
}

void 
switch_init()			/* setup switch */
{  
  P2REN |= SWITCHES;		/* enables resistors for switches */
  P2IE |= SWITCHES;		/* enable interrupts from switches */
  P2OUT |= SWITCHES;		/* pull-ups for switches */
  P2DIR &= ~SWITCHES;		/* set switches' bits for input */
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
            if (button4){
                clearScreen(BLACK);
                transition(WAITING);
            }
    }
}


// axis zero for col, axis 1 for row

short drawPos_1[2] = {1,10}, controlPos_1[2] = {2, 10};
short drawPos_2[2] = {50,40}, controlPos_2[2] = {51, 40};
short colVelocity = 1, colLimits[2] = {1, screenWidth};

void
draw_ball(int col, int row, unsigned short color)
{
    fillRectangle(col, row, 30, 7, color);
    fillRectangle(col+5, row-5, 15, 5, color);
    fillRectangle(col+20, row-2, 5, 2, color);
}


void
screen_update_ball(short drawPos[2], short controlPos[2])
{
  for (char axis = 0; axis < 2; axis ++)
    if (drawPos[axis] != controlPos[axis]) /* position changed? */
      goto redraw;
  return;			/* nothing to do */
 redraw:
  draw_ball(drawPos[0], drawPos[1], COLOR_WHITE); /* erase */
  for (char axis = 0; axis < 2; axis ++)
    drawPos[axis] = controlPos[axis];
  draw_ball(drawPos[0], drawPos[1], COLOR_GRAY); /* draw */
}
  

short redrawScreen = 1;
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
//  if (secCount >= 25) {		/* 10/sec */
//   
//    {				/* move ball */
//      short oldCol = controlPos[0];
//      short newCol = oldCol + colVelocity;
//      if (newCol <= colLimits[0] || newCol >= colLimits[1])
//	colVelocity = -colVelocity;
//      else
//	controlPos[0] = newCol;
//    }
//
//    {				/* update hourglass */
//      if (switches & SW3) green = (green + 1) % 64;
//      if (switches & SW2) blue = (blue + 2) % 32;
//      if (switches & SW1) red = (red - 3) % 32;
//      if (step <= 30)
//	step ++;
//      else
//	step = 0;
//      secCount = 0;
//    }
//    if (switches & SW4) return;
//    redrawScreen = 1;
//  }

  
//void update_shape();
void main()
{
  
  P1DIR |= LED;		/**< Green led on when CPU on */
  P1OUT |= LED;
  configureClocks();
  lcd_init();
  switch_init();
  
  enableWDTInterrupts();      /**< enable periodic interrupt */
  or_sr(0x8);	              /**< GIE (enable interrupts) */
  
  clearScreen(BLACK);
    update_time(0,0);
    clock_colon();
    
    P1OUT &= ~LED;	/* led off */
    or_sr(0x10);	/**< CPU OFF */
    P1OUT |= LED;	/* led on */
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
  screen_update_ball(drawPos_1, controlPos_1);
  screen_update_ball(drawPos_2, controlPos_2);
  //screen_update_hourglass();
}
   


void
__interrupt_vec(PORT2_VECTOR) Port_2(){
  if (P2IFG & SWITCHES) {	      /* did a button cause this interrupt? */
    P2IFG &= ~SWITCHES;		      /* clear pending sw interrupts */
    switch_interrupt_handler();	/* single handler for all switches */
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
        clearScreen(WHITE);
        transition(GAME);
    }
}

void game(){
    update_shape();
}

void move_clouds() {
    secCount++;
    if (secCount >= 10) { /* 10/sec */
        secCount = 0;
        /* move ball */
        short oldCol_one = controlPos_1[0];
        short newCol_one = oldCol_one + colVelocity;
        
        short oldCol_two = controlPos_2[0];
        short newCol_two = oldCol_two + colVelocity;
        if (newCol_one <= colLimits[0] || newCol_one >= colLimits[1]) {

            if (newCol_one <= colLimits[0])
                controlPos_1[0] = colLimits[1]; // move to the other end
            else
                controlPos_1[0] = colLimits[0]; // move to the other end
        } else
            controlPos_1[0] = newCol_one;
        
        
        if (newCol_two <= colLimits[0] || newCol_two >= colLimits[1]) {

            if (newCol_two <= colLimits[0])
                controlPos_2[0] = colLimits[1]; // move to the other end
            else
                controlPos_2[0] = colLimits[0]; // move to the other end
        } else
            controlPos_2[0] = newCol_two;

    }
}
