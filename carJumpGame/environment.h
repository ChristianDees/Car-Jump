#ifndef environment_included
#define environment_included

void draw_road(int col, int row, unsigned short color);
void screen_update_road(short drawPos[2], short controlPos[2]);

void draw_cloud(int col, int row, unsigned short color);
void screen_update_cloud(short drawPos[2], short controlPos[2]);
void move_clouds();
void floor_bar();

extern short drawPos_road_one[2];
extern short drawPos_road_two[2];

extern short controlPos_road_one[2];
extern short controlPos_road_two[2];

extern short drawPos_cloud[2];
extern short drawPos_cloud_two[2];

extern short controlPos_cloud[2];
extern short controlPos_cloud_two[2];


extern short env_seconds;
extern char floor_done;

#endif
