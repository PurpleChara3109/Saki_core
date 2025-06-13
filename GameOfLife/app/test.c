/*
 * test.c
 *
 * Created on: May 27, 2025
 * Author: Valéry et Neïssa
 */

#include "test.h"
#include "gameoflife.h"
#include "actions.h"
#include "menu.h"
#include "button.h"
#include "display.h"
#include "TFT_ili9341/stm32g4_ili9341.h"
#include "TFT_ili9341/stm32g4_xpt2046.h"
#include "TFT_ili9341/stm32g4_fonts.h"
#include "stm32g4xx_hal.h"
#include <stdio.h>
#include <stdbool.h>

extern grid_t grid0;
extern volatile bool is_paused;
extern user_actions_e action;
extern volatile bool in_menu;
extern menu_e selected_menu;
extern pattern_e selected_pattern;
extern volatile bool in_pattern_menu;


typedef enum {
    TEST_INIT_DISPLAY,
    TEST_GLIDER_GEN,
    TEST_PLAY_ACTION,
    TEST_GENERATIONS_RUN,
    TEST_PAUSE_ACTION,
    TEST_CLEAR_GRID_ACTION,
    TEST_MENU_OPEN,
    TEST_MENU_NAVIGATE,
    TEST_MENU_SELECT_PATTERN,
    TEST_PATTERN_MENU_NAVIGATE,
    TEST_PATTERN_CREATE,
    TEST_END_SUMMARY,
} test_state_e;

static test_state_e current_test_state = TEST_INIT_DISPLAY;
static uint32_t test_state_entry_time_ms = 0;

/**
 * @brief Test de l'écran uniquement
 */
void TEST_screen(void)
{
    ILI9341_Fill(ILI9341_COLOR_BLACK);
    ILI9341_Puts(50, 50, "Test concluant pour l'ecran :)", &Font_7x10, ILI9341_COLOR_WHITE, ILI9341_COLOR_BLACK);

}

/**
 * @brief Test de l'écran tactile
 */
void TEST_tactile(void)
{
	XPT2046_demo();

}

/**
 * @brief Affiche un message de test sur la console série
 * @param message Le message à afficher.
 * @param success Indique si le test associé a réussi (true) ou échoué (false).
 */
void test_log(const char* message, bool success) {

    printf("[TEST] %s: %s\r\n", message, success ? "SUCCES" : "ECHEC");
}

/**
 * @brief Fonction principale de la machine à états de test.
 * Cette fonction doit être appelée en continu dans la boucle 'main'
 * lorsque le mode test est activé.
 */
void TEST_state_machine_run(void) {
    if (HAL_GetTick() - test_state_entry_time_ms < 1000 && current_test_state != TEST_INIT_DISPLAY) {
        return;
    }

    switch (current_test_state) {
        case TEST_INIT_DISPLAY:
            ILI9341_Fill(ILI9341_COLOR_BLACK);
            test_log("--- Demarrage Suite de Tests ---", true);
            ILI9341_Puts(0, 50, "Les tests vont s'executer automatiquement.", &Font_7x10, ILI9341_COLOR_WHITE, ILI9341_COLOR_BLACK);
            test_state_entry_time_ms = HAL_GetTick();
            current_test_state = TEST_GLIDER_GEN;
            break;

        case TEST_GLIDER_GEN:
            test_log("Test: Generation d'un Glider", true);
            DISPLAY_refresh(&grid0);
            DISPLAY_sync_with_grid(&grid0);
            GAMEOFLIFE_create_pattern(&grid0, 10, 10, PATTERN_GLIDER);
            DISPLAY_refresh(&grid0);
            test_state_entry_time_ms = HAL_GetTick();
            current_test_state = TEST_PLAY_ACTION;
            break;

        case TEST_PLAY_ACTION:
            test_log("Test: Action Play", true);
            is_paused = true;
            action_play_pause();
            bool play_success = !is_paused;
            test_log("   Jeu demarre ?", play_success);
            test_state_entry_time_ms = HAL_GetTick();
            current_test_state = TEST_GENERATIONS_RUN;
            break;

        case TEST_GENERATIONS_RUN:
            if (HAL_GetTick() - test_state_entry_time_ms < 5000) {
                return;
            }
            test_log("Test: Generations en cours (visuel)", true);
            test_state_entry_time_ms = HAL_GetTick();
            current_test_state = TEST_PAUSE_ACTION;
            break;

        case TEST_PAUSE_ACTION:
            test_log("Test: Action Pause", true);
            is_paused = false;
            action_play_pause();
            bool pause_success = is_paused;
            test_log("   Jeu mis en pause ?", pause_success);
            test_state_entry_time_ms = HAL_GetTick();
            current_test_state = TEST_CLEAR_GRID_ACTION;
            break;

        case TEST_CLEAR_GRID_ACTION:
            test_log("Test: Action Clear Grid", true);
            action_clear_grid();
            bool grid_clear_success = true;
            for (int x = 0; x < NB_COLUMN; x++) {
                for (int y = 0; y < NB_LINE; y++) {
                    if (grid0.cells[x][y] != CELL_DEAD) {
                        grid_clear_success = false;
                        break;
                    }
                }
                if (!grid_clear_success) break;
            }
            test_log("   Grille videe ?", grid_clear_success);
            DISPLAY_refresh(&grid0);
            test_state_entry_time_ms = HAL_GetTick();
            current_test_state = TEST_MENU_OPEN;
            break;

        case TEST_MENU_OPEN:
            test_log("Test: Action Ouvrir Menu", true);
            action_open_menu();
            bool menu_open_success = in_menu;
            test_log("   Menu ouvert ?", menu_open_success);
            test_log("   Menu selectionne: Play/Pause ?", selected_menu == MENU_PLAY_PAUSE);
            test_state_entry_time_ms = HAL_GetTick();
            current_test_state = TEST_MENU_NAVIGATE;
            break;

        case TEST_MENU_NAVIGATE:
            test_log("Test: Navigation Menu principal", true);
            selected_menu = MENU_CLEAR_GRID;
            MENU_display();
            HAL_Delay(500);
            selected_menu = MENU_SAVE_GRID;
            MENU_display();
            HAL_Delay(500);

            bool nav_success = (selected_menu == MENU_SAVE_GRID);
            test_log("   Menu navigue a 'Save Grid' ?", nav_success);
            test_state_entry_time_ms = HAL_GetTick();
            current_test_state = TEST_MENU_SELECT_PATTERN;
            break;

        case TEST_MENU_SELECT_PATTERN:
            test_log("Test: Selection Menu Pattern", true);
            selected_menu = MENU_SELECT_PATTERN;
            action = ACTION_SELECT_PATTERNS;
            action_to_do(action);

            bool pattern_menu_open_success = in_pattern_menu;
            test_log("   Menu patterns ouvert ?", pattern_menu_open_success);
            test_state_entry_time_ms = HAL_GetTick();
            current_test_state = TEST_PATTERN_MENU_NAVIGATE;
            break;

        case TEST_PATTERN_MENU_NAVIGATE:
            test_log("Test: Navigation Menu Pattern", true);
            selected_pattern = PATTERN_TOAD;
            HAL_Delay(500);
            selected_pattern = PATTERN_GLIDER;
            HAL_Delay(500);

            bool pattern_nav_success = (selected_pattern == PATTERN_GLIDER);
            test_log("   Pattern navigue a 'Glider' ?", pattern_nav_success);
            test_state_entry_time_ms = HAL_GetTick();
            current_test_state = TEST_PATTERN_CREATE;
            break;

        case TEST_PATTERN_CREATE:
            test_log("Test: Creation Pattern selectionne", true);
            action = ACTION_CREATE_PATTERNS;
            action_to_do(action);

            bool pattern_create_success = (grid0.cells[10][10] == CELL_ALIVE &&
                                          grid0.cells[11][10] == CELL_ALIVE &&
                                          grid0.cells[12][10] == CELL_ALIVE);
            test_log("   Glider cree sur la grille ?", pattern_create_success);
            DISPLAY_refresh(&grid0);
            HAL_Delay(2000);

            test_state_entry_time_ms = HAL_GetTick();
            current_test_state = TEST_END_SUMMARY;
            break;

        case TEST_END_SUMMARY:
            ILI9341_Fill(ILI9341_COLOR_BLACK);
            test_log("--- Suite de Tests Terminee ---", true);
            ILI9341_Puts(0, 50, "Tous les tests se sont executes.", &Font_7x10, ILI9341_COLOR_WHITE, ILI9341_COLOR_BLACK);
            printf("\r\n--- FIN DE LA SUITE DE TESTS ---\r\n");
            while(1);
            break;
    }
}
