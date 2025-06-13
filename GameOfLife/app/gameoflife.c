                                                                                                                                                 /*
 * sokoban.c
 *
 *  Created on: Mar 11, 2024
 *      Author: Nirgal
 */

/**
 * @brief Fonction principale qui gère le cadencement et le fonctionne de l'application.
 */

#include "gameoflife.h"
#include "stm32g4xx_hal.h"
#include "button.h"
#include "actions.h"
#include "display.h"
#include "menu.h"
#include "stm32g4_systick.h"
#include "TFT_ili9341/stm32g4_xpt2046.h"
#include "TFT_ili9341/stm32g4_ili9341.h"
#include "TFT_ili9341/stm32g4_xpt2046.h"  // For a touchscreen use

//Definition of constants
#define NB_LEVELS 11
#define GENERATION_PERIOD_MS 100

//Variables privées types
static grid_t grid1;
static volatile bool flag_generation = false; // Détermine si l'on doit passer à la génération suivante
static button_e down, center;
static bool flag_first_enter;

// Variables publiques
grid_t grid0;

user_actions_e action;
volatile bool is_paused;
volatile bool in_menu;
volatile bool in_pattern_menu;
uint8_t cooldown_after_menu = 0;

typedef enum{
	GOF_INIT,
	GOF_START,
	GOF_IN_MENU,
	GOF_IN_PATTERN_MENU,
	GOF_PLAY,
	GOF_PAUSE,
	GOF_SAVE
} state_game;

//Prototypes of private functions
static void GAMEOFLIFE_process_ms(void);
static void GAMEOFLIFE_next_generation(grid_t * grid_prev, grid_t * grid_next);
static void GAMEOFLIFE_touchscreen(void);
void GAMEOFLIFE_create_pattern(grid_t * grid, uint16_t x, uint16_t y, pattern_e pattern);
static void GAMEOFLIFE_state_machine(void);

//Public functions

/**
 * @brief Initialisation de notre Jeu de la vie.
 * On initialise l'écran LCD, le fonctionnalité tactile, ainsi que les boutons.
 */
void GAMEOFLIFE_init(void)
{
	// Initialisation des éléments importants
	DISPLAY_init();		// initialisation de l'écran
	BUTTONS_init();		// initialisations des boutons utilisés
	XPT2046_init();		// initialisation de l'écran tactil
	BSP_systick_add_callback_function(GAMEOFLIFE_process_ms); // On appelle la fonction en paramètre à chaque instant

	// Variables nécessaires pour la création d'un pattern (cf méthode GAMEOFLIFE_create_pattern)

	grid_t *p = &grid0;

	// On cherche à initialiser la grille des cellules avec, au début, uniquement des cellules mortes

	cell_e basis_cell = CELL_DEAD;

	// On utilise le principe de buffering en remplissant une première grille d'affichage

	for(int x = 0; x < NB_COLUMN; x++){

		for(int y =0; y < NB_LINE; y++){

			grid0.cells[x][y] = basis_cell;
		}
	}

	// Affichage en fournissant l'adresse de grid0

	DISPLAY_refresh(p);

	in_pattern_menu = false;
	in_menu = false;
	is_paused = true;
	flag_first_enter = false;
}

/**
 * Fonction matérialisant le processus principal de notre jeu.
 */
void GAMEOFLIFE_process_main(void)
{
	static bool toggle = false;
	grid_t *grid_prev;
	grid_t *grid_next;

	BUTTONS_update(&down, &center);

	GAMEOFLIFE_state_machine();

	if(flag_first_enter)
	{
		ILI9341_Puts(50, 100, "Conway's Game Of Life", &Font_11x18, ILI9341_COLOR_WHITE, ILI9341_COLOR_WHITE);
		flag_first_enter = false;

	}

	// Cadensement des générations
	if(!is_paused && flag_generation){
		if(!toggle){
			grid_prev = &grid0;
			grid_next = &grid1;
			toggle = true;
		}else{
			grid_prev = &grid1;
			grid_next = &grid0;
			toggle = false;
		}
		// Appel des fonctions pour la next gen et la rafraîchissement de l'écran

		GAMEOFLIFE_next_generation(grid_prev, grid_next);
		DISPLAY_refresh(grid_next);

		// On abaisse finalement le flag

		flag_generation = false;
	}

	if(!in_menu && !in_pattern_menu)
	{
		GAMEOFLIFE_touchscreen();  // L'écran tactile
	}
}

/**
 * @brief Machine à états modélisant les états de notre projets.
 */
void GAMEOFLIFE_state_machine(void)
{
	static state_game current_state = GOF_INIT;
	static state_game previous_state;

	previous_state = current_state;

	switch(current_state)
	{
		case GOF_INIT:
			ILI9341_Puts(50, 100, "Conway's Game Of Life", &Font_11x18, ILI9341_COLOR_PINK, ILI9341_COLOR_WHITE);

			if(center == BUTTON_PRESS_EVENT){
				flag_first_enter = true;
				current_state = GOF_START;
			}
			break;
		case GOF_START: // on gére tout ici
            if (previous_state == GOF_INIT || previous_state == GOF_IN_MENU || previous_state == GOF_IN_PATTERN_MENU || previous_state == GOF_PAUSE)
			{
				in_pattern_menu = false;
				in_menu = false;
				is_paused = true;
			}

//			if(previous_state == GOF_INIT)
//			{
//				flag_first_enter = false;
//			}

			if(center == BUTTON_PRESS_EVENT && cooldown_after_menu == 0)
			{
				action_open_menu(); // Appelle MENU_open() qui met in_menu_global à true
				current_state = GOF_IN_MENU;
				cooldown_after_menu = 10; // Empêche l'ouverture/fermeture rapide
			}
			break;
		case GOF_IN_MENU:
			if (previous_state != GOF_IN_MENU) { // Actions à l'entrée de l'état
				MENU_open(); // Affiche le menu
				is_paused = true;
				in_menu = true;
				in_pattern_menu = false; // S'assurer que le drapeau pattern est faux
			}

			MENU_handle_input(down, center); // Gère la navigation et sélection du menu

			// Traitement de l'action sélectionnée
			switch(action)
			{
				case ACTION_PLAY_PAUSE:
					action_play_pause(); // Cela va inverser is_paused_global
					current_state = is_paused ? GOF_PAUSE : GOF_PLAY; // Transition en fonction de l'état de pause
					action = ACTION_NONE; // Consommer l'action
					break;
				case ACTION_CLEAR_GRID:
					action_clear_grid();
					current_state = GOF_START; // Après nettoyage, retourne à l'état START
					action = ACTION_NONE; // Consommer l'action
					break;
				case ACTION_SAVE_GRID:
					action_save_grid();
					current_state = GOF_START; // Après sauvegarde, retourne à l'état START
					action = ACTION_NONE; // Consommer l'action
					break;
				case ACTION_SELECT_PATTERNS:
					action_open_patterns_menu(); // Cela va mettre in_pattern_menu_global à true
					current_state = GOF_IN_PATTERN_MENU;
					action = ACTION_NONE;
					break;
				default:
					break;
			}
			break;

		case GOF_IN_PATTERN_MENU:
			if (previous_state != GOF_IN_PATTERN_MENU) { // Actions à l'entrée de l'état
				PATTERN_MENU_open(); // Affiche le menu des patterns
				is_paused = true;
				in_menu = false;
				in_pattern_menu = true;
			}

			PATTERN_MENU_handle_input(down, center); // Gère la navigation et sélection du pattern

			if(action == ACTION_CREATE_PATTERNS) { // L'action de création de pattern est validée
				action_create_patterns(); // Applique le pattern sur la grille
				current_state = GOF_START; // Retourne à l'état START pour permettre de continuer à dessiner ou jouer
				action = ACTION_NONE; // Consommer l'action
			}
			break;

		case GOF_PLAY:
			if (previous_state != GOF_PLAY) { // Actions à l'entrée de l'état
				is_paused = false;
				in_menu = false;
				in_pattern_menu = false;
				// Transition d'écran si nécessaire (déjà fait par action_play_pause)
			}
			// Si le bouton central est pressé pendant le jeu, passer en pause
			if(center == BUTTON_PRESS_EVENT && cooldown_after_menu == 0){
				action_play_pause(); // Cela va inverser is_paused_global à true
				current_state = GOF_PAUSE;
				cooldown_after_menu = 10;
			}
			break;

		case GOF_PAUSE: // Le jeu est en pause, la grille est visible et modifiable
			if (previous_state != GOF_PAUSE) { // Actions à l'entrée de l'état
				is_paused = true;
				in_menu = false;
				in_pattern_menu = false;
				// Transition d'écran si nécessaire (déjà fait par action_play_pause)
			}
			// Si le bouton central est pressé, revenir à l'état START (qui permet d'ouvrir le menu)
			if(center == BUTTON_PRESS_EVENT && cooldown_after_menu == 0){
				action_play_pause(); // Cela va inverser is_paused_global à false et passer en PLAY
				current_state = GOF_PLAY;
				cooldown_after_menu = 10;
			}
			// L'utilisateur peut aussi ouvrir le menu depuis PAUSE
			if(center == BUTTON_PRESS_EVENT && cooldown_after_menu == 0) {
				action_open_menu();
				current_state = GOF_IN_MENU;
				cooldown_after_menu = 10;
			}
			break;

		case GOF_SAVE:
			in_menu = false;
			if(action == ACTION_NONE)
				current_state = GOF_START;
			break;

	}

}

/**
 * Fonction permettant de créer des patterns prédéfinies.
 */
void GAMEOFLIFE_create_pattern(grid_t * grid, uint16_t x, uint16_t y, pattern_e pattern)
{
	switch(pattern)
	{
		case PATTERN_BOX:
			grid->cells[x+0][y+0] = CELL_ALIVE;
			grid->cells[x+0][y+1] = CELL_ALIVE;
			grid->cells[x+1][y+0] = CELL_ALIVE;
			grid->cells[x+1][y+1] = CELL_ALIVE;
			break;
		case PATTERN_BEEHIVE:
			grid->cells[x+2][y+4] = CELL_ALIVE;
			grid->cells[x+3][y+3] = CELL_ALIVE;
			grid->cells[x+3][y+5] = CELL_ALIVE;
			grid->cells[x+4][y+3] = CELL_ALIVE;
			grid->cells[x+5][y+4] = CELL_ALIVE;
			grid->cells[x+4][y+5] = CELL_ALIVE;
			break;
	    case PATTERN_TOAD:
	    	grid->cells[x+1][y+2] = CELL_ALIVE;
			grid->cells[x+1][y+3] = CELL_ALIVE;
			grid->cells[x+1][y+4] = CELL_ALIVE;
			grid->cells[x+2][y+1] = CELL_ALIVE;
			grid->cells[x+2][y+2] = CELL_ALIVE;
			grid->cells[x+2][y+3] = CELL_ALIVE;
			break;
		case PATTERN_SHIP:
			grid->cells[x+2][y+2] = CELL_ALIVE;
			grid->cells[x+2][y+3] = CELL_ALIVE;
			grid->cells[x+3][y+2] = CELL_ALIVE;
			grid->cells[x+4][y+3] = CELL_ALIVE;
			grid->cells[x+4][y+4] = CELL_ALIVE;
			grid->cells[x+3][y+4] = CELL_ALIVE;
			break;
		case PATTERN_GLIDER:
			grid->cells[x+8][y + 1] = CELL_ALIVE;
			grid->cells[x+7][4] = CELL_ALIVE;
			grid->cells[x+6][4] = CELL_ALIVE;
			grid->cells[x+7][5] = CELL_ALIVE;
			grid->cells[x+8][5] = CELL_ALIVE;
			break;
	    case PATTERN_QUEEN_BEE_SHUTTLE:
	    	grid->cells[x+0][y+8] = CELL_ALIVE;
			grid->cells[x+0][y+9] = CELL_ALIVE;
			grid->cells[x+1][y+10] = CELL_ALIVE;
			grid->cells[x+2][y+11] = CELL_ALIVE;
			grid->cells[x+3][y+11] = CELL_ALIVE;
			grid->cells[x+4][y+11] = CELL_ALIVE;
			grid->cells[x+5][y+10] = CELL_ALIVE;
			grid->cells[x+6][y+9] = CELL_ALIVE;
			grid->cells[x+6][y+8] = CELL_ALIVE;
			break;
		case PATTERN_PULSAR:
			grid->cells[x+2][y+4] = CELL_ALIVE;
			grid->cells[x+3][y+3] = CELL_ALIVE;
			grid->cells[x+3][y+4] = CELL_ALIVE;
			grid->cells[x+3][y+5] = CELL_ALIVE;
			grid->cells[x+4][y+3] = CELL_ALIVE;
			grid->cells[x+4][y+5] = CELL_ALIVE;
			grid->cells[x+5][y+3] = CELL_ALIVE;
			grid->cells[x+5][y+4] = CELL_ALIVE;
			grid->cells[x+5][y+5] = CELL_ALIVE;
			grid->cells[x+6][y+4] = CELL_ALIVE;
			break;
		case PATTERN_BLINKER:
			grid->cells[x+2][y+2] = CELL_ALIVE;
			grid->cells[x+2][y+3] = CELL_ALIVE;
			grid->cells[x+2][y+4] = CELL_ALIVE;
		break;
		case PATTERN_PENTADECATHLON:
			grid->cells[x+5][y+0] = CELL_ALIVE;
			grid->cells[x+5][y+1] = CELL_ALIVE;
			grid->cells[x+5][y+2] = CELL_ALIVE;
			grid->cells[x+5][y+3] = CELL_ALIVE;
			grid->cells[x+5][y+4] = CELL_ALIVE;
			grid->cells[x+5][y+5] = CELL_ALIVE;
			grid->cells[x+5][y+6] = CELL_ALIVE;
			grid->cells[x+5][y+7] = CELL_ALIVE;
			grid->cells[x+5][y+8] = CELL_ALIVE;
			grid->cells[x+5][y+9] = CELL_ALIVE;
			break;
		case PATTERN_GLIDER_GUN:
			grid->cells[x+0][y+3] = CELL_ALIVE;
			grid->cells[x+0][y+4] = CELL_ALIVE;
			grid->cells[x+1][y+3] = CELL_ALIVE;
			grid->cells[x+1][y+4] = CELL_ALIVE;
			grid->cells[x+10][y+2] = CELL_ALIVE;
			grid->cells[x+10][y+3] = CELL_ALIVE;
			grid->cells[x+10][y+4] = CELL_ALIVE;
			grid->cells[x+11][y+1] = CELL_ALIVE;
			grid->cells[x+11][y+5] = CELL_ALIVE;
			grid->cells[x+12][y+0] = CELL_ALIVE;
			grid->cells[x+12][y+6] = CELL_ALIVE;
			grid->cells[x+13][y+0] = CELL_ALIVE;
			grid->cells[x+13][y+6] = CELL_ALIVE;
			grid->cells[x+14][y+3] = CELL_ALIVE;
			grid->cells[x+15][y+1] = CELL_ALIVE;
			grid->cells[x+15][y+5] = CELL_ALIVE;
			grid->cells[x+16][y+2] = CELL_ALIVE;
			grid->cells[x+16][y+3] = CELL_ALIVE;
			grid->cells[x+16][y+4] = CELL_ALIVE;
			grid->cells[x+17][y+3] = CELL_ALIVE;
			grid->cells[x+20][y+4] = CELL_ALIVE;
			grid->cells[x+20][y+5] = CELL_ALIVE;
			grid->cells[x+20][y+6] = CELL_ALIVE;
			grid->cells[x+21][y+4] = CELL_ALIVE;
			grid->cells[x+21][y+5] = CELL_ALIVE;
			grid->cells[x+21][y+6] = CELL_ALIVE;
			grid->cells[x+22][y+3] = CELL_ALIVE;
			grid->cells[x+22][y+7] = CELL_ALIVE;
			grid->cells[x+24][y+2] = CELL_ALIVE;
			grid->cells[x+24][y+3] = CELL_ALIVE;
			grid->cells[x+24][y+7] = CELL_ALIVE;
			grid->cells[x+24][y+8] = CELL_ALIVE;
			grid->cells[x+34][y+5] = CELL_ALIVE;
			grid->cells[x+34][y+6] = CELL_ALIVE;
			grid->cells[x+35][y+5] = CELL_ALIVE;
			grid->cells[x+35][y+6] = CELL_ALIVE;
			break;
		case PATTERN_LIGHT_WEIGHT_SPACESHIP:
			grid->cells[x][y+1] = CELL_ALIVE;
			grid->cells[x][y+2] = CELL_ALIVE;
			grid->cells[x+1][y+1] = CELL_ALIVE;
			grid->cells[x+1][y+2] = CELL_ALIVE;
			grid->cells[x+1][y+3] = CELL_ALIVE;
			grid->cells[x+2][y+0] = CELL_ALIVE;
			grid->cells[x+2][y+2] = CELL_ALIVE;
			grid->cells[x+2][y+3] = CELL_ALIVE;
			grid->cells[x+3][y+0] = CELL_ALIVE;
			grid->cells[x+3][y+1] = CELL_ALIVE;
			grid->cells[x+3][y+2] = CELL_ALIVE;
			grid->cells[x+4][y+1] = CELL_ALIVE;

			break;
		case PATTERN_MIDDLE_WEIGHT_SPACESHIP:
			grid->cells[x+0][y+1] = CELL_ALIVE;
			grid->cells[x+0][y+2] = CELL_ALIVE;
			grid->cells[x+1][y+0] = CELL_ALIVE;
			grid->cells[x+1][y+1] = CELL_ALIVE;
			grid->cells[x+1][y+2] = CELL_ALIVE;
			grid->cells[x+2][y+0] = CELL_ALIVE;
			grid->cells[x+2][y+1] = CELL_ALIVE;
			grid->cells[x+2][y+2] = CELL_ALIVE;
			grid->cells[x+3][y+0] = CELL_ALIVE;
			grid->cells[x+3][y+1] = CELL_ALIVE;
			grid->cells[x+3][y+3] = CELL_ALIVE;
			grid->cells[x+4][y+1] = CELL_ALIVE;
			grid->cells[x+4][y+2] = CELL_ALIVE;
			grid->cells[x+4][y+3] = CELL_ALIVE;
			grid->cells[x+5][y+2] = CELL_ALIVE;
			break;
		case PATTERN_HEAVY_WEIGHT_SPACESHIP:
			grid->cells[x+0][y+1] = CELL_ALIVE;
			grid->cells[x+0][y+2] = CELL_ALIVE;
			grid->cells[x+1][y+0] = CELL_ALIVE;
			grid->cells[x+1][y+1] = CELL_ALIVE;
			grid->cells[x+1][y+2] = CELL_ALIVE;
			grid->cells[x+2][y+0] = CELL_ALIVE;
			grid->cells[x+2][y+1] = CELL_ALIVE;
			grid->cells[x+2][y+2] = CELL_ALIVE;
			grid->cells[x+3][y+0] = CELL_ALIVE;
			grid->cells[x+3][y+1] = CELL_ALIVE;
			grid->cells[x+3][y+2] = CELL_ALIVE;
			grid->cells[x+4][y+0] = CELL_ALIVE;
			grid->cells[x+4][y+1] = CELL_ALIVE;
			grid->cells[x+4][y+3] = CELL_ALIVE;
			grid->cells[x+5][y+1] = CELL_ALIVE;
			grid->cells[x+5][y+2] = CELL_ALIVE;
			grid->cells[x+5][y+3] = CELL_ALIVE;
			grid->cells[x+6][y+2] = CELL_ALIVE;
			break;
		default:
			break;
	}
}

//Private functions
static void GAMEOFLIFE_process_ms(void) // Cette fonction ne s'implémente nulle part, elle sert juste
// à lever le flag
{
	static uint8_t t_generation;

//	On l'incrémente avec modulo -- cela sert à pouvoir effectuer une action cyclique

	t_generation = (t_generation + 1) % GENERATION_PERIOD_MS;

	if(t_generation == 0)
		flag_generation = true; // On lève le flag pour passer à la génération suivante chaque 100ms

	if (cooldown_after_menu > 0)
	    cooldown_after_menu--;

}

/**
 * @brief Gère le cadencement des générations
 * @param grid_prev est la grille précédente.
 * @param grid_next est la nouvelle grille, qui est calculée simultanément dans un buffer;
 */
static void GAMEOFLIFE_next_generation(grid_t * grid_prev, grid_t * grid_next)
{
	// Variables la cellule actuelle, la suivante
	// le nbre de cellule mortes/vivantes autour

//	uint8_t nbre_cell_dead = 0;
	cell_e cell_main;
	cell_e cell_next;

	// On parcours dans un premier temps tout l'écran

	for(int x = 0; x < NB_COLUMN; x++){
		for(int y = 0; y < NB_LINE; y++){
			cell_main = grid_prev->cells[x][y]; // On récup la cellule actuelle, puis on analyse les 8
			cell_next = cell_main;
			//aux alentours en parcourant le carré
			uint8_t nbre_cell_alive = 0;
			for(int x_count = -1; x_count <= 1; x_count++){
				for(int y_count = -1; y_count <= 1; y_count++){

					if(x_count == 0 && y_count == 0)
						continue;

					// Je récupère ainsi la position de chaque cellue du carré, en ignorant celle du milieu
					int x_neighbor = x + x_count;
					int y_neighbor = y + y_count;

					// On vérifie qu'on est toujours dans les limites de l'écran, sinon la cellule
					// est ignorée ( la voisine, pas la cellule qu'on analyse)

					if (x_neighbor >= 0 && x_neighbor < NB_COLUMN && y_neighbor >= 0 && y_neighbor < NB_LINE) {
						if(grid_prev->cells[x_neighbor][y_neighbor] == CELL_ALIVE){
							nbre_cell_alive++;
						}
					}
				}
			}
			// Après avoir compté le nombre de cellule mortes, on applique les règles du jeu de la vie

			if(cell_main == CELL_ALIVE){
				if(nbre_cell_alive < 2){
					cell_next = CELL_DEAD; // Mort par solitude (la pauvre)
				}else if(nbre_cell_alive == 2 || nbre_cell_alive == 3){
					cell_next = CELL_ALIVE; // vive la fraternitude
				}else if(nbre_cell_alive > 3){
					cell_next = CELL_DEAD; // Elle n'aime pas la populace
				}
			}else{
				if(nbre_cell_alive == 3){
					cell_next = CELL_ALIVE; // Résurrection par la FAME
				}
			}

			// On actualise maintenant l'état de la cellule dans la grille suivante
			grid_next->cells[x][y] = cell_next;
		}
	}

}

/**
 * On permet à l'utilisateur d'intéragir avec l'écran tactile
 * Les variables last_x et last_y empêche un appui long d'inverser constamment l'état des cellules
 * Pourquoi -1 ? Comme ça, au démarrage ou entre deux appuis, rien ne peut se passer ;)
 */

static void GAMEOFLIFE_touchscreen(void)
{

    static int16_t last_x = -1;
    static int16_t last_y = -1;
    int16_t x, y = 0;


    if(XPT2046_getMedianCoordinates(&x, &y, XPT2046_COORDINATE_SCREEN_RELATIVE))
    {

        // Éviter de re-toucher la même cellule si on garde le doigt appuyé
        if(x == last_x && y == last_y)
            return;

        last_x = x;
        last_y = y;

        // Coordonnées de cellule
        uint16_t cell_x = x / CELL_SIZE; // Pour récupérer correctement les cooordonnées
        uint16_t cell_y = y / CELL_SIZE;

        if(cell_x < NB_COLUMN && cell_y < NB_LINE)
        {
            // Inverser état de la cellule
            grid0.cells[cell_x][cell_y] = (grid0.cells[cell_x][cell_y] == CELL_ALIVE) ? CELL_DEAD : CELL_ALIVE;

            DISPLAY_refresh(&grid0);
        }
    }
    else
    {
        last_x = -1;
        last_y = -1;
    }
}


