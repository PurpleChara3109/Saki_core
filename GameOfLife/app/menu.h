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
	MENU_OPEN_SETTINGS,
	MENU_SELECT_PATTERN,
	MENU_NB   // celui là sert pour les boucles for ( à voir si je le garde ou pas)
} menu_e;

extern user_actions_e action;

void MENU_display(void);
void MENU_open(void);
void MENU_handle_input(button_e left, button_e center);

#endif
