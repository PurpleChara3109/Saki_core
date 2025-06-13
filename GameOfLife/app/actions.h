#ifndef ACTIONS_H_
#define ACTIONS_H_

#include "button.h"
#include "gameoflife.h"
#include <stdbool.h>


extern grid_t grid0;
extern uint8_t cooldown_after_menu;
extern pattern_e selected_pattern;

extern volatile bool is_paused; // Assurez-vous que c'est bien la même variable que dans gameoflife.c
extern volatile bool in_menu; // Assurez-vous que c'est bien la même variable que dans gameoflife.c
extern volatile bool in_pattern_menu;
/*
    prototypes des fonctions présentes dans actions.c
*/
void action_play_pause(void);
void action_clear_grid(void);
void action_save_grid(void);
void action_open_menu(void);
void action_open_patterns_menu(void);
void action_create_patterns(void);
void action_load_grid(void);

void action_to_do(user_actions_e action);

#endif /* ACTIONS_H_ */

