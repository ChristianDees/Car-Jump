#ifndef clock_included
#define clock_included

void refresh_1();
void refresh_2();
void refresh_3();
void refresh_4();
void update_time(char change_hour, char change_minutes);
void count_time();
void display_clock();
void blink_change_clock();
void screen_update_time();


extern short hour;
extern short minutes;
extern char changeTime;
extern char display_clock_once;
extern char refresh_time_flag;
extern char hour_flag;
extern char minutes_flag;

extern int clock_seconds;
extern int totalSeconds;
extern int blink_seconds;
#endif
