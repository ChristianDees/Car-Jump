#ifndef environment_included
#define environment_included

void draw_road(int col, int row, unsigned short color);
void screen_update_road(short drawPos[2], short controlPos[2]);

void draw_ball(int col, int row, unsigned short color);
void screen_update_ball(short drawPos[2], short controlPos[2]);
void move_clouds();
void floor_bar();

extern short drawPos_road_one[2];
extern short drawPos_road_two[2];

extern short controlPos_road_one[2];
extern short controlPos_road_two[2];

extern short drawPos_one[2];
extern short drawPos_two[2];

extern short controlPos_one[2];
extern short controlPos_two[2];

extern short count;
extern char floor_done;

#endif
