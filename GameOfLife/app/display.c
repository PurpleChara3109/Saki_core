/*
 * display.c
 *
 *  Created on: Feb 12, 2024
 *      Author: Nirgal
 */
#include "gameoflife.h"
#include "stm32g4xx_hal.h"
#include "stm32g4_systick.h"
#include "display.h"
#include "TFT_ili9341/stm32g4_ili9341.h"
#include "TFT_ili9341/stm32g4_fonts.h"



//Constantes privées
static cell_e displayed_cells[NB_COLUMN][NB_LINE] = {{CELL_DEAD}};

//Prototype des fonctions privées
static bool reset = false;

//_____________________________________

//Fonctions publiques

void DISPLAY_init(void) {
	ILI9341_Init();

	// Pour régler le rafraîchissement de l'écran
	for(int x = 0; x < NB_COLUMN; x++) {
		for(int y = 0; y < NB_LINE; y++) {
			displayed_cells[x][y] = CELL_DEAD;
		}
	}

	reset = true;
}



void DISPLAY_refresh(grid_t *grid){
	//Code qui permet de calculer l'état de chaque cellule de la grille


	for(int x = 0; x < NB_COLUMN; x++){
		for(int y = 0; y < NB_LINE; y++){

			// De la manière suivante, je ne rafraîchit que les cellules changeantes
			if((reset == true) || (grid->cells[x][y] != displayed_cells[x][y])){
				// Affichage de la cellule

				// Et on remplit les cellules avec du blanc, en premier lieu, puis on vérifie si elles sont vivantes ou pas
				uint16_t color = ILI9341_COLOR_MAGENTA;

				    if(grid->cells[x][y] == CELL_ALIVE)
				        color = ILI9341_COLOR_BLACK;
				    else
				        color = ILI9341_COLOR_WHITE;

				    ILI9341_DrawFilledRectangle(
				        x * CELL_SIZE,
				        y * CELL_SIZE,
				        (x + 1) * CELL_SIZE,
				        (y + 1) * CELL_SIZE,
				        color
				    );

				    displayed_cells[x][y] = grid->cells[x][y];
			}
		}
	}
	// On force "reset" à false à la fin

	reset = false;

}

// Approche de solution

void DISPLAY_sync_with_grid(grid_t *grid)
{
    for(int x = 0; x < NB_COLUMN; x++)
    {
        for(int y = 0; y < NB_LINE; y++)
        {
            displayed_cells[x][y] = ~grid->cells[x][y];  // force une différence
        }
    }
}

//_____________________________________

//Fonctions privées

