#include <msp430.h>
#include "lcdutils.h"
#include "lcddraw.h"
#include "updates.h"

short hour = 0;
short minutes = 0;
char changeTime = 0;
char display_clock_once = 1;
int clock_seconds = 0;
int total_seconds = 0;


// refresh first and second position
void refresh_1(){
    short first_digit_h = hour/10;
    short second_digit_h = hour % 10;
    clock_number(0,1,1);
    clock_number(0,2,1);
    clock_number(first_digit_h,1,0);
    clock_number(second_digit_h,2,0);
}

// refresh only second position
void refresh_2(){
    short second_digit_h = hour % 10;
    clock_number(0,2,1);
    clock_number(second_digit_h,2,0);
}

// refresy third and fourth position
void refresh_3(){
    short first_digit_m = minutes/10;
    short second_digit_m = minutes % 10;
    clock_number(0,3,1);
    clock_number(0,4,1);
    clock_number(first_digit_m,3,0);
    clock_number(second_digit_m,4,0);
}

// refresh only fourth position
void refresh_4(){
    clock_number(0,4,1);
    short second_digit_m = minutes % 10;
    clock_number(second_digit_m,4,0);
}

// change time
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
        else if (second_digit_h <= 9 && hour > 0)
            refresh_2();
    }
    if (change_minutes){
        if (minutes == 0 || (minutes%10 == 0))
            refresh_3();
        else if (second_digit_m <= 9 && minutes > 0)
            refresh_4();
    }
    else if (!(change_hour | change_minutes)){
        clock_number(first_digit_h, 1,0);
        clock_number(second_digit_h, 2,0);
        clock_number(first_digit_m, 3,0);
        clock_number(second_digit_m, 4,0);
    }
    redrawScreen = 1;
}

// show current time
void display_clock(){
    if(display_clock_once){
        clearScreen(COLOR_GRAY);
        update_time(0,0);
        clock_colon();
        redrawScreen = 1;
        display_clock_once = 0;
    }
    clock_seconds++;
      if (clock_seconds >= 250){
          clock_seconds = 0;
          total_seconds++;
          if (total_seconds >= 60){
              total_seconds = 0;
              minutes++;
              update_time(0,1);
          }
      }
      if (minutes >= 60){
          hour++;
          update_time(1,0);
      }
    redrawScreen = 1;
}

// blink screen on and off every second
void blink_change_clock(){
    clock_seconds++;
    if(clock_seconds >= 250){
        clock_seconds = 0;
        refresh_1();
        refresh_3();
    }
    redrawScreen = 1;
}
