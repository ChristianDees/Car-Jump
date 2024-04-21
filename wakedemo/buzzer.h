#ifndef buzzer_included
#define buzzer_included

// prototype functions
void buzzer_init();
void buzzer_set_period(short cycles);
void buzz_game_over();

// external vars
extern int buzz_seconds;
extern char buzz_second_count;
extern char buzz_toggler;
extern char buzz_flag;

#endif
