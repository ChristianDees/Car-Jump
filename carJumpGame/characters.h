#ifndef characters_included
#define characters_included

void move_enemy();
void draw_character(int col, int row, unsigned short color1, unsigned short color2, unsigned short color3, unsigned short color4);
void screen_update_character();
void character_jump();
void draw_enemy(int col, int row, unsigned short color1, unsigned short color2, unsigned short color3, unsigned short color4);
void screen_update_enemy(unsigned int colorBGR);
void screen_update_enemy_two(unsigned int colorBGR);

extern short drawPosCharacter[2];
extern short controlPosCharacter[2];

extern short colLimitsCharacter[2];

extern short drawPos_enemy[2];
extern short drawPos_enemy_two[2];

extern short controlPos_enemy[2];
extern short controlPos_enemy_two[2];

extern char enable_second_enemy;
extern char overlap_flag;
extern char jump_flag;
extern short enemy_seconds;
extern short jump_seconds;
extern short post_secs;
extern short jump_once;

extern unsigned int colorBGR;
extern unsigned int colorBGR_two;

#endif
