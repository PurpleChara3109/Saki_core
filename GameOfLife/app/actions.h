#ifndef ACTIONS_H_
#define ACTIONS_H_

#include "button.h"
#include "gameoflife.h"
#include <stdbool.h>

extern bool is_paused;
extern bool in_menu;
extern bool in_settings;

extern grid_t grid0;
/*
    prototypes des fonctions présentes dans actions.c
*/
void action_play_pause(void);
void action_clear_grid(void);
void action_save_grid(void);
void action_open_menu(void);
void action_open_settings(void);
void action_to_do(user_actions_e action);

#endif /* ACTIONS_H_ */

