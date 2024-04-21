#include <msp430.h>
#include "lcdutils.h"
#include "lcddraw.h"
#include "updates.h"

short hour = 0;
short minutes = 0;
char changeTime = 0;
char display_clock_once = 1;
int secCount = 0;
int totalSeconds = 0;

void refresh_1(){
    short first_digit_h = hour/10;
    short second_digit_h = hour % 10;
    clock_number(0,1,1);
    clock_number(0,2,1);
    clock_number(first_digit_h,1,0);
    clock_number(second_digit_h,2,0);
}
void refresh_2(){
    short second_digit_h = hour % 10;

    clock_number(0,2,1);
    clock_number(second_digit_h,2,0);
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
    short second_digit_m = minutes % 10;
    clock_number(second_digit_m,4,0);
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
        if (hour == 0 || (second_digit_h == 0))
            refresh_1();
        else if (second_digit_h <= 9 && hour > 0){
            refresh_2();
        }
    }
    if (change_minutes){
        
        if (minutes == 0 || (minutes%10 == 0))
            refresh_3();
        else if (second_digit_m <= 9 && minutes > 0){
            refresh_4();
            
        }
    }
    else if (!(change_hour | change_minutes)){
        clock_number(first_digit_h, 1,0);
        clock_number(second_digit_h, 2,0);
        clock_number(first_digit_m, 3,0);
        clock_number(second_digit_m, 4,0);
    }
    redrawScreen = 1;
}


void display_clock(){
    if(display_clock_once){
        clearScreen(COLOR_GRAY);
        update_time(0,0);
        clock_colon();
        redrawScreen = 1;
        display_clock_once = 0;
    }

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
