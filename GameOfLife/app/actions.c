#include "actions.h"
#include "config.h"
#include "TFT_ili9341/stm32g4_fonts.h"
#include "TFT_ili9341/stm32g4_ili9341.h"
#include "menu.h"
#include "display.h"

// Variables globales
bool is_paused = true;
bool in_menu = false;
bool in_settings = false;

extern grid_t grid0;

/**
 * Fonction pour effectuer une transition verticale
 */
void TRANSITION_vertical_wipe(uint16_t color, uint16_t step_delay)
{
	for (int y = 0; y < ILI9341_HEIGHT; y += 10)
	{
		ILI9341_DrawFilledRectangle(0, y, ILI9341_WIDTH, y + 10, color);
		HAL_Delay(step_delay);
	}
}

/**
 * Applique une transition après mise à jour logique
 */
void TRANSITION_apply_with_grid(grid_t *grid, uint16_t color, uint16_t step_delay)
{
	TRANSITION_vertical_wipe(color, step_delay);
	ILI9341_Fill(ILI9341_COLOR_WHITE);          // Nettoyage visuel
	DISPLAY_sync_with_grid(grid);               // Réalignement mémoire <-> écran
	DISPLAY_refresh(grid);                     // Redessin complet
}

/**
 * Action : jouer / mettre en pause la simulation
 */
void action_play_pause(void)
{
	is_paused = !is_paused;
	in_menu = false;

	extern uint8_t cooldown_after_menu;
	cooldown_after_menu = 10;

	TRANSITION_vertical_wipe(ILI9341_COLOR_WHITE, 10); // Ajouté AVANT le rafraîchissement
	ILI9341_Fill(ILI9341_COLOR_WHITE);                 // Nettoyage post-transition
	DISPLAY_sync_with_grid(&grid0);
	DISPLAY_refresh(&grid0);

//	if (is_paused)
//	{
//		ILI9341_DrawLine(111, 150, 6, 20, ILI9341_COLOR_PINK);
//		ILI9341_DrawLine(122, 150, 6, 20, ILI9341_COLOR_PINK);
//	}
//	else
//	{
//		ILI9341_DrawLine(111, 150, 6, 20, ILI9341_COLOR_WHITE);
//		ILI9341_DrawLine(122, 150, 6, 20, ILI9341_COLOR_WHITE);
//	}
}

/**
 * Action : efface toutes les cellules de la grille
 */
void action_clear_grid(void)
{
	in_menu = false;

	extern uint8_t cooldown_after_menu;
	cooldown_after_menu = 10;

	for (int x = 0; x < NB_COLUMN; x++)
		for (int y = 0; y < NB_LINE; y++)
			grid0.cells[x][y] = CELL_DEAD;

	TRANSITION_apply_with_grid(&grid0, ILI9341_COLOR_WHITE, 10);
}

/**
 * Action : sauvegarde (à implémenter)
 */
void action_save_grid(void) {}

/**
 * Action : ouvre le menu
 */
void action_open_menu(void)
{
	MENU_open();
}

/**
 * Action : ouvre les paramètres
 */
void action_open_settings(void)
{
	in_settings = true;
	ILI9341_Fill(ILI9341_COLOR_WHITE);
	ILI9341_Puts(70, 150, "SETTINGS", &Font_11x18, ILI9341_COLOR_BLACK, ILI9341_COLOR_WHITE);
}

/**
 * Action : permet de choisir un pattern à afficher
 */
void action_create_patterns(void)
{
	in_menu = false;

	extern uint8_t cooldown_after_menu;
	cooldown_after_menu = 10;

	// ⚙️ Place ici tous les patterns que tu veux afficher
	GAMEOFLIFE_create_pattern(&grid0, 10, 10, PATTERN_BOX);
	GAMEOFLIFE_create_pattern(&grid0, 20, 10, PATTERN_BEEHIVE);
	GAMEOFLIFE_create_pattern(&grid0, 30, 10, PATTERN_TOAD);
	GAMEOFLIFE_create_pattern(&grid0, 40, 10, PATTERN_BLINKER);
	GAMEOFLIFE_create_pattern(&grid0, 60, 50, PATTERN_SHIP);
	GAMEOFLIFE_create_pattern(&grid0, 70, 50, PATTERN_PULSAR);
	GAMEOFLIFE_create_pattern(&grid0, 10, 40, PATTERN_GLIDER_GUN);
	GAMEOFLIFE_create_pattern(&grid0, 2, 2, PATTERN_LIGHT_WEIGHT_SPACESHIP);
	GAMEOFLIFE_create_pattern(&grid0, 5, 10, PATTERN_MIDDLE_WEIGHT_SPACESHIP);
	GAMEOFLIFE_create_pattern(&grid0, 5, 20, PATTERN_HEAVY_WEIGHT_SPACESHIP);

	// 💡 Rafraîchissement visuel propre
	ILI9341_Fill(ILI9341_COLOR_WHITE);
	DISPLAY_sync_with_grid(&grid0);
	DISPLAY_refresh(grid0);
}

/**
 * Dispatcher général d’actions utilisateur
 */
void action_to_do(user_actions_e action)
{
	switch (action)
	{
	case ACTION_PLAY_PAUSE:
		action_play_pause();
		break;
	case ACTION_CLEAR_GRID:
		action_clear_grid();
		break;
	case ACTION_SAVE_GRID:
		action_save_grid();
		break;
	case ACTION_OPEN_MENU:
		action_open_menu();
		break;
	case ACTION_OPEN_SETTINGS:
		action_open_settings();
		break;
	default:
		break;
	}
}
