#ifndef hud_included
#define hud_included

extern int high_score;
extern int current_score;
extern char display_pause_once;
extern char reset_pause_flag;
extern char display_controls_once;

extern char score_once;
extern char seconds_score;

extern char draw_once;
extern char pause_flag;


void display_controls();
void display_controls_two();
void display_score();
void screen_update_score();
void display_pause();
void reset_pause();
void draw_game_over();




#endif
