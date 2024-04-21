#ifndef stateMachines_included
#define stateMachines_included

// states
typedef enum {
    INTRO,
    WAITING,
    CHANGETIME,
    CONTROLPAGEONE,
    CONTROLPAGETWO,
    GAME,
    PAUSE,
    GAMEOVER
} State;

// function prototypes
void state_intro();
void state_waiting();
void state_change_time();
void state_control_page_one();
void state_control_page_two();
void state_game();
void state_pause();
void state_game_over();
void transition(State next_state);

// external vars
extern State current_state;

#endif
