/*
 * @brief Permet de gérer le menu disponibles et la navigation entre ceux ci.
 * @Author Neïssa et Valéry
 */

#include "menu.h"
#include "config.h"
#include "button.h"
#include "TFT_ili9341/stm32g4_ili9341.h"
#include "TFT_ili9341/stm32g4_fonts.h"
#include "gameoflife.h"

// Variables privées
static const char* menu_labels[MENU_NB] = {		// affichage à l'écran en fonction du menu (avec const char, ces chaînes sont non modifiables
	"Play/Pause",
	"Clear Grid",
	"Save grid",
	"Load Grid",
	"Select Pattern"
};

// Pour la sélection de patterns

pattern_e selected_pattern = PATTERN_BOX;

const char* pattern_labels[PATTERN_NB] = {
    "Box", "Toad", "Beehive", "Blinker", "Ship", "Pulsar",
    "Glider Gun", "LW Spaceship", "MW Spaceship","HW Spaceship"
};

menu_e selected_menu = MENU_PLAY_PAUSE;  // menu sélectionné

// Fonctions

static void PATTERN_MENU_display(void);

/**
 * @brief Ouvre le menu.
 */
void MENU_open(void)
{
	in_menu = true;
	selected_menu = MENU_PLAY_PAUSE;  // reset
	MENU_display();
}

/**
 * @brief: Fonction pour afficher le menu à l'écran
 * On affiche à l'écran tous les menus accessibles
 */
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
 * @brief Navigue dans le menu avec le bouton du bas.
 * Chaque appui passe à l'élément suivant (boucle circulaire).
 * @param nav: pour naviguer dans le menu
 * @param validate: pour sélectionner le menu
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
            case MENU_SELECT_PATTERN:
            	action = ACTION_SELECT_PATTERNS;
            	break;
            default: break;
        }
        changed = true;
    }

    if(changed && in_menu)
        MENU_display();  // Rafraîchit l'affichage uniquement si nécessaire
}

/**
 * Ouvre le menu de choix de patterns.
 */
void PATTERN_MENU_open(void)
{
	in_menu = false;
	in_pattern_menu = true;
	PATTERN_MENU_display();
}

/**
 * Pour afficher le menu de sélection de patterns
 */
static void PATTERN_MENU_display(void)
{
    ILI9341_Fill(ILI9341_COLOR_WHITE);
    for(int i = 0; i < PATTERN_NB; i++)
    {
        uint16_t y = 20 + i * 20;
        uint16_t color = (i == selected_pattern) ? ILI9341_COLOR_BLUE : ILI9341_COLOR_BLACK;
        ILI9341_Puts(20, y, pattern_labels[i], &Font_7x10, color, ILI9341_COLOR_WHITE);
    }
}

/**
 * @brief Navigue dans le menu de choix de patterns avec le bouton du bas.
 * Chaque appui passe à l'élément suivant (boucle circulaire).
 * @param nav: pour naviguer dans le menu
 * @param validate: pour sélectionner le patterns
 */
void PATTERN_MENU_handle_input(button_e nav, button_e validate)
{
    bool changed = false;

    if(nav == BUTTON_PRESS_EVENT)
    {
        selected_pattern = (selected_pattern + 1) % PATTERN_NB;
        changed = true;
    }

    if(validate == BUTTON_PRESS_EVENT)
    {
    	action = ACTION_CREATE_PATTERNS;
        in_pattern_menu = false;
        in_menu = false;
        changed = true;
    }

    if(changed && in_pattern_menu)
        PATTERN_MENU_display();
}




