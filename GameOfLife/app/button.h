/*
 * button.h
 *
 *  Created on: Feb 12, 2024
 *      Author: Nirgal
 */

 #ifndef BUTTON_H_
 #define BUTTON_H_

 #include "stm32g4xx_hal.h"
 //possibles événements des boutons
 typedef enum
 {
     BUTTON_NO_EVENT,
     BUTTON_PRESS_EVENT,
     BUTTON_RELEASE_EVENT
 }button_e;

 //les actions réaliasasbles par l'utilisateur
 typedef enum{
    ACTION_NONE,
    ACTION_PLAY_PAUSE,
    ACTION_CLEAR_GRID,
    ACTION_SAVE_GRID,
    ACTION_OPEN_MENU,
	ACTION_SELECT_PATTERNS,
	ACTION_CREATE_PATTERNS,
 }user_actions_e;

 void BUTTONS_init(void);
 void BUTTONS_update(
                     button_e * button_down_event,
                     button_e * button_center_event);


 uint8_t BUTTON_down_read(void);
 uint8_t BUTTON_center_read(void);


 #endif /* BUTTON_H_ */

