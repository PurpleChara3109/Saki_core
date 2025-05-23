/*
 * Créé par Neïssa et Valéry
 */

#include "menu.h"
#include "config.h"
#include "button.h"
#include "TFT_ili9341/stm32g4_ili9341.h"
#include "TFT_ili9341/stm32g4_fonts.h"
#include "gameoflife.h"

// Variables privées
static menu_e selected_menu = MENU_PLAY_PAUSE;  // menu sélectionné
static const char* menu_labels[MENU_NB] = {		// affichage à l'écran en fonction du menu (avec const char, ces chaînes sont non modifiables
	"Play/Pause",
	"Clear Grid",
	"Settings",
	"Select Pattern"
};
extern user_actions_e action;
// Fonctions

/**
 * @brief: Fonction pour afficher le menu à l'écran
 * On affiche à l'écran tous les menus accessibles
 */

void MENU_open(void)
{
	in_menu = true;
	selected_menu = MENU_PLAY_PAUSE;  // reset
	MENU_display();
}

void MENU_display(void)
{
	ILI9341_Fill(ILI9341_COLOR_WHITE);
	for(int i = 0; i < MENU_NB; i++)
	{
		uint16_t y = 40 + i * 30;
		uint16_t color = (i == selected_menu) ? ILI9341_COLOR_PINK : ILI9341_COLOR_BLACK;
		ILI9341_Puts(60, y, menu_labels[i], &Font_11x18, color, ILI9341_COLOR_WHITE);
	}
}

/**
 * @brief Navigue dans le menu avec le bouton gauche uniquement.
 * Chaque appui passe à l'élément suivant (boucle circulaire).
 * @param right: événement du bouton haut (BUTTON_PRESS_EVENT, etc.)
 * @param left: événement du bouton bas
 * @param center: événement du bouton central (validation)
 */
void MENU_handle_input(button_e nav, button_e validate)
{
    bool changed = false; // Indique si un changement a eu lieu (navigation ou validation) pour rafraîchir l'écran

    if(nav == BUTTON_PRESS_EVENT)
    {
        selected_menu = (selected_menu + 1) % MENU_NB;
        changed = true;
    }

    if(validate == BUTTON_PRESS_EVENT)
    {
        switch(selected_menu)
        {
            case MENU_PLAY_PAUSE:
                action = ACTION_PLAY_PAUSE;
                break;
            case MENU_CLEAR_GRID:
                action = ACTION_CLEAR_GRID;
                break;
            case MENU_OPEN_SETTINGS:
                action = ACTION_OPEN_SETTINGS;
                break;
            default: break;
        }
        changed = true;
    }

    if(changed && in_menu)
        MENU_display();  // Rafraîchit l'affichage uniquement si nécessaire
}




