/**
 * Crée par Neïssa et Valéry
 *
 */

#ifndef MENU_H_
#define MENU_H_

// Fichiers utiles pour la config des menu

#include "actions.h"

// Variables et structures/énumérations utiles

typedef enum {
	MENU_PLAY_PAUSE,
	MENU_CLEAR_GRID,
	MENU_SAVE_GRID,
	MENU_LOAD_GRID,
	MENU_SELECT_PATTERN,
	MENU_NB   // celui là sert pour les boucles for ( à voir si je le garde ou pas)
} menu_e;

extern menu_e selected_menu;  // menu sélectionné

extern user_actions_e action;

extern volatile bool is_paused; // Assurez-vous que c'est bien la même variable que dans gameoflife.c
extern volatile bool in_menu; // Assurez-vous que c'est bien la même variable que dans gameoflife.c
extern volatile bool in_pattern_menu;

void MENU_display(void);
void MENU_open(void);
void MENU_handle_input(button_e left, button_e center);
void PATTERN_MENU_handle_input(button_e nav, button_e validate);
void PATTERN_MENU_open(void);


#endif
