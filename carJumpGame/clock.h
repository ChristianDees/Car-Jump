#ifndef clock_included
#define clock_included

void refresh_1();
void refresh_2();
void refresh_3();
void refresh_4();
void update_time(char change_hour, char change_minutes);
void display_clock();
void blink_change_clock();


extern short hour;
extern short minutes;
extern char changeTime;
extern char display_clock_once;

extern int secCount;
extern int totalSeconds;
#endif
