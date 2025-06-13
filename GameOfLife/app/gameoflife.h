/*
 * breakbricks.h
 *
 *  Created on: Feb 12, 2024
 *      Author: Nirgal
 */
#include <stdbool.h>
#include <stdint.h>

#ifndef GAMEOFLIFE_H_
#define GAMEOFLIFE_H_

#define SCREEN_WIDTH	320
#define SCREEN_HEIGHT	240
#define CELL_SIZE 		4


#define NB_COLUMN		SCREEN_WIDTH/CELL_SIZE
#define NB_LINE			SCREEN_HEIGHT/CELL_SIZE
#define NB_CELLS 		(NB_COLUMN*NB_LINE)

// État d'une cellule

typedef enum {              // En langage micro contrôleur, il n'y a pas '#typedef'
	CELL_DEAD = 0,
	CELL_ALIVE = 1} cell_e;

// Grille sur laquelle évolue des cellules

typedef struct{
	cell_e cells [NB_COLUMN][NB_LINE];} grid_t;


/*		^		column
 * 		| y		  v
 * 		_____________________________________
 * 		|__|__|__|__|__|__|__|__|__|__|__|__|
* 		|__|__|__|__|__|__|__|__|__|__|__|__|
* 		|__|__|__|__|__|__|__|__|__|__|__|__|		<---- line
* 		|__|__|__|__|__|__|__|__|__|__|__|__|
* 		|__|__|__|__|__|__|__|__|__|__|__|__|
* 		|__|__|__|__|__|__|__|__|__|__|__|__|
* 		|__|__|__|__|__|__|__|__|__|__|__|__|
* 		|__|__|__|__|__|__|__|__|__|__|__|__|
* 		|__|__|__|__|__|__|__|__|__|__|__|__|
* 		|__|__|__|__|__|__|__|__|__|__|__|__|
* 		|__|__|__|__|__|__|__|__|__|__|__|__|
* 		o__|__|__|__|__|__|__|__|__|__|__|__|--->   x
*
*/

typedef enum
{
	PATTERN_BOX,
	PATTERN_TOAD,
	PATTERN_BEEHIVE,
	PATTERN_BLINKER,
	PATTERN_SHIP,
	PATTERN_PULSAR,
	PATTERN_GLIDER_GUN,
	PATTERN_LIGHT_WEIGHT_SPACESHIP,
	PATTERN_MIDDLE_WEIGHT_SPACESHIP,
	PATTERN_HEAVY_WEIGHT_SPACESHIP,
	PATTERN_QUEEN_BEE_SHUTTLE,
	PATTERN_PENTADECATHLON,
	PATTERN_GLIDER,
	PATTERN_NB
}pattern_e;

extern pattern_e selected_pattern;

void GAMEOFLIFE_init(void);

void GAMEOFLIFE_process_main(void);

void GAMEOFLIFE_create_pattern(grid_t * grid, uint16_t x, uint16_t y, pattern_e pattern);


#endif /* GAMEOFLIFE_H_ */
