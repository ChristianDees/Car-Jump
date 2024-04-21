#ifndef buzzer_included
#define buzzer_included

// prototype functions
void buzzer_init();
void buzzer_set_period(short cycles);
void intro_buzz();
void buzz_game_over();
void jump_buzz();

// external vars
extern int buzz_seconds;
extern char buzz_second_count;
extern char buzz_toggler;
extern char buzz_flag;
extern char jump_buzz_flag;
extern int jump_buzz_seconds;

#endif
