/**
 * @brief Gère les différentes actions disponibles et effectuées par l'utilisateur.
 * @Author : Valéry et Neïssa
 */

#include "actions.h"
#include "config.h"
#include "TFT_ili9341/stm32g4_fonts.h"
#include "TFT_ili9341/stm32g4_ili9341.h"
#include "menu.h"
#include "display.h"

// Variables globales

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
 * @brief Sauvegarde la grille actuelle dans la mémoire Flash.
 *
 * VÉRIFIEZ IMPÉRATIVEMENT que l'adresse 0x0801C000 et les 3 pages suivantes
 * (0x0801C000 à 0x0801E7FF) sont libres de tout code de votre firmware.
 * UN EFFACEMENT ICI PEUT CORROMPRE VOTRE MICROCONTRÔLEUR.
 */
void action_save_grid(void)
{
    HAL_FLASH_Unlock();
    FLASH_EraseInitTypeDef EraseInitStruct;
    uint32_t PageError = 0; // Variable pour stocker les erreurs d'effacement

    EraseInitStruct.TypeErase   = FLASH_TYPEERASE_PAGES;
    EraseInitStruct.Banks       = FLASH_BANK_1;

    // Calcul de la page de début à partir de l'adresse de base du module Flash
    // (0x0801C000 - 0x08000000) / 2048 octets/page = 56

    EraseInitStruct.Page        = (0x0801C000 - FLASH_BASE) / FLASH_PAGE_SIZE;
    EraseInitStruct.NbPages     = 3; // 3 pages nécessaires pour stocker 4800 octets (80x60 cellules)

    HAL_StatusTypeDef erase_status = HAL_FLASHEx_Erase(&EraseInitStruct, &PageError);
    HAL_FLASH_Lock();

    if (erase_status != HAL_OK) {
        // Gérer l'erreur d'effacement si nécessaire (par exemple, afficher un message d'erreur)
        ILI9341_Puts(10, 10, "Flash Erase Error!", &Font_7x10, ILI9341_COLOR_RED, ILI9341_COLOR_WHITE);
        HAL_Delay(2000);
        DISPLAY_refresh(&grid0);
        return;
    }

    // Ecriture
    uint32_t flash_doubleword_index = 0;
    uint64_t data_to_write = 0;
    uint8_t cell_in_dw_count = 0; // Compteur de cellules par double mot

    for (int y = 0; y < NB_LINE; y++)
    {
        for (int x = 0; x < NB_COLUMN; x++)
        {
            // Chaque cellule est un 0 (CELL_DEAD) ou 1 (CELL_ALIVE).
            // On les stocke sur 1 octet pour simplifier l'accès, même si 1 bit suffirait.
            data_to_write |= ((uint64_t)grid0.cells[x][y] << (cell_in_dw_count * 8));
            cell_in_dw_count++;

            if (cell_in_dw_count == 8) // Quand 8 cellules (8 octets) sont empaquetées dans un uint64_t
            {
                BSP_FLASH_set_doubleword(flash_doubleword_index, data_to_write);
                flash_doubleword_index++;
                data_to_write = 0; // Réinitialise pour le prochain lot
                cell_in_dw_count = 0;
            }
        }
    }
    // S'il reste des cellules non écrites (moins de 8 à la fin de la grille)
    if (cell_in_dw_count > 0)
    {
        BSP_FLASH_set_doubleword(flash_doubleword_index, data_to_write);
    }

    is_paused = true; // Met en pause après sauvegarde
    in_menu = false;
    DISPLAY_refresh(&grid0); // Rafraîchit l'écran avec la grille après le message
    ILI9341_Puts(10, 10, "Grille sauvegardee!", &Font_7x10, ILI9341_COLOR_GREEN, ILI9341_COLOR_WHITE);
    HAL_Delay(1000); // Afficher un message de confirmation
}

/**
 * @brief Charge la grille depuis la mémoire Flash.
 */
void action_load_grid(void)
{
    uint32_t flash_doubleword_index = 0;
    uint64_t loaded_data = 0;
    uint8_t cell_in_dw_count = 0;

    for (int y = 0; y < NB_LINE; y++)
    {
        for (int x = 0; x < NB_COLUMN; x++)
        {
            if (cell_in_dw_count == 0) // Lire un nouveau double mot si on a épuisé le précédent
            {
                loaded_data = BSP_FLASH_read_doubleword(flash_doubleword_index);
                flash_doubleword_index++;
            }

            // Extraire l'état de la cellule du double mot chargé (un octet)
            grid0.cells[x][y] = (cell_e)((loaded_data >> (cell_in_dw_count * 8)) & 0xFF);
            cell_in_dw_count++;

            if (cell_in_dw_count == 8) // Réinitialise le compteur pour le prochain double mot
            {
                cell_in_dw_count = 0;
            }
        }
    }

    is_paused = true; // Met en pause après chargement
    in_menu = false;
    DISPLAY_refresh(&grid0); // Rafraîchit l'affichage de la grille chargée
    ILI9341_Puts(10, 10, "Grille chargee!", &Font_7x10, ILI9341_COLOR_BLUE, ILI9341_COLOR_WHITE);
    HAL_Delay(1000); // Afficher un message de confirmation
}


/**
 * Action : ouvre le menu
 */
void action_open_menu(void)
{
	MENU_open();
}

/**
 * Pour choisir un pattern précis !
 */
void action_open_patterns_menu(void)
{
	PATTERN_MENU_open();
}

/**
 * Action : permet de choisir un pattern à afficher
 */
void action_create_patterns(void)
{
	in_menu = false;

	extern uint8_t cooldown_after_menu;
	cooldown_after_menu = 10;

	ILI9341_Fill(ILI9341_COLOR_WHITE);

	switch(selected_pattern)
	{
	case PATTERN_BOX:
		GAMEOFLIFE_create_pattern(&grid0, 10, 10, PATTERN_BOX);
		break;
	case PATTERN_TOAD:
		GAMEOFLIFE_create_pattern(&grid0, 30, 10, PATTERN_TOAD);
		break;
	case PATTERN_BEEHIVE:
		GAMEOFLIFE_create_pattern(&grid0, 20, 10, PATTERN_BEEHIVE);
		break;
	case PATTERN_BLINKER:
		GAMEOFLIFE_create_pattern(&grid0, 40, 10, PATTERN_BLINKER);
		break;
	case PATTERN_SHIP:
		GAMEOFLIFE_create_pattern(&grid0, 60, 50, PATTERN_SHIP);
		break;
	case PATTERN_PULSAR:
		GAMEOFLIFE_create_pattern(&grid0, 70, 50, PATTERN_PULSAR);
		break;
	case PATTERN_GLIDER_GUN:
		GAMEOFLIFE_create_pattern(&grid0, 10, 40, PATTERN_GLIDER_GUN);
		break;
	case PATTERN_LIGHT_WEIGHT_SPACESHIP:
		GAMEOFLIFE_create_pattern(&grid0, 2, 2, PATTERN_LIGHT_WEIGHT_SPACESHIP);
		break;
	case PATTERN_MIDDLE_WEIGHT_SPACESHIP:
		GAMEOFLIFE_create_pattern(&grid0, 5, 10, PATTERN_MIDDLE_WEIGHT_SPACESHIP);
		break;
	case PATTERN_HEAVY_WEIGHT_SPACESHIP:
		GAMEOFLIFE_create_pattern(&grid0, 5, 20, PATTERN_HEAVY_WEIGHT_SPACESHIP);
		break;
	default:
		break;
	}

// Rafraîchissement visuel propre
	DISPLAY_sync_with_grid(&grid0);
	DISPLAY_refresh(&grid0);
}

/**
 * Sélectionne l'action à effectuer.
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
	case ACTION_SELECT_PATTERNS:
		action_open_patterns_menu();
		break;
	case ACTION_CREATE_PATTERNS:
		action_create_patterns();
		break;
	default:
		break;
	}
}
