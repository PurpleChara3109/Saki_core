/*
 * button.c
 *
 *  Created on: Feb 12, 2024  & april 04,2025
 *      Author: Nirgal & Neïssa
*/

#include "stm32g4xx_hal.h"
#include "stm32g4_systick.h"
#include "button.h"
#include "actions.h"
#include "stm32g4_gpio.h"

// les define sont là pour nous dire à quelle broche et quel pin sont branchés nos différents boutons
//exemple(du dessous): Le bouton gauche est branché à la broche(pin) 12 du port A
#define GPIO_BUTTON_LEFT GPIOA
#define PIN_BUTTON_LEFT GPIO_PIN_12     //button erase

#define GPIO_BUTTON_RIGHT GPIOB
#define PIN_BUTTON_RIGHT GPIO_PIN_6		//button save

#define GPIO_BUTTON_UP GPIOA
#define PIN_BUTTON_UP GPIO_PIN_10		//button menu

#define GPIO_BUTTON_DOWN GPIOA
#define PIN_BUTTON_DOWN GPIO_PIN_9 		//button settings

#define GPIO_BUTTON_CENTER GPIOB
#define PIN_BUTTON_CENTER GPIO_PIN_0 	//button play-pause


//cette fonction configure chaque bouton comme une entrée avec résistance
void BUTTONS_process_ms(void);
static volatile uint32_t t = 0;


//cette fonction permet de créer un retard entre deux lectures pour éviter de lire trop vite et détecter les rebonds
void BUTTONS_init(void)
{
	//Initialisation du port du bouton bleu (carte Nucleo)
	BSP_GPIO_pin_config(GPIO_BUTTON_LEFT, PIN_BUTTON_LEFT, GPIO_MODE_INPUT, GPIO_PULLUP, GPIO_SPEED_FREQ_HIGH, GPIO_MODE_INPUT);
	BSP_GPIO_pin_config(GPIO_BUTTON_RIGHT, PIN_BUTTON_RIGHT, GPIO_MODE_INPUT, GPIO_PULLUP, GPIO_SPEED_FREQ_HIGH, GPIO_MODE_INPUT);
	BSP_GPIO_pin_config(GPIO_BUTTON_UP, PIN_BUTTON_UP, GPIO_MODE_INPUT, GPIO_PULLUP, GPIO_SPEED_FREQ_HIGH, GPIO_MODE_INPUT);
	BSP_GPIO_pin_config(GPIO_BUTTON_DOWN, PIN_BUTTON_DOWN, GPIO_MODE_INPUT, GPIO_PULLUP, GPIO_SPEED_FREQ_HIGH, GPIO_MODE_INPUT);
	BSP_GPIO_pin_config(GPIO_BUTTON_CENTER, PIN_BUTTON_CENTER, GPIO_MODE_INPUT, GPIO_PULLUP, GPIO_SPEED_FREQ_HIGH, GPIO_MODE_INPUT);
	BSP_systick_add_callback_function(&BUTTONS_process_ms);
}


void BUTTONS_process_ms(void)
{
	if(t)
		t--;
}


//les fonctions servent à lire l'état électrique des différents boutons
uint8_t BUTTON_left_read(void)
{
	return !HAL_GPIO_ReadPin(GPIO_BUTTON_LEFT, PIN_BUTTON_LEFT);
}

uint8_t BUTTON_right_read(void)
{
	return !HAL_GPIO_ReadPin(GPIO_BUTTON_RIGHT, PIN_BUTTON_RIGHT);
}

uint8_t BUTTON_up_read(void)
{
	return !HAL_GPIO_ReadPin(GPIO_BUTTON_UP, PIN_BUTTON_UP);
}

uint8_t BUTTON_down_read(void)
{
	return !HAL_GPIO_ReadPin(GPIO_BUTTON_DOWN, PIN_BUTTON_DOWN);
}

uint8_t BUTTON_center_read(void)
{
	return !HAL_GPIO_ReadPin(GPIO_BUTTON_CENTER, PIN_BUTTON_CENTER);
}


//La fonction sert à dire pour chaque bouton : “est-ce qu’il vient d’être pressé ? relâché ? ou rien du tout ?”
void BUTTONS_update(button_e * button_left_event, button_e * button_right_event, button_e * button_up_event, button_e * button_down_event, button_e * button_center_event)
{
	static bool previous_button_left = false;
	static bool previous_button_right = false;
	static bool previous_button_up = false;
	static bool previous_button_down = false;
	static bool previous_button_center = false;

	*button_left_event = BUTTON_NO_EVENT;
	*button_right_event = BUTTON_NO_EVENT;
	*button_up_event = BUTTON_NO_EVENT;
	*button_down_event = BUTTON_NO_EVENT;
	*button_center_event = BUTTON_NO_EVENT;

	if(!t)
	{
		t = 20;
		bool current_button_left = 	!HAL_GPIO_ReadPin(GPIO_BUTTON_LEFT, 	PIN_BUTTON_LEFT);
		bool current_button_right = 	!HAL_GPIO_ReadPin(GPIO_BUTTON_RIGHT, 	PIN_BUTTON_RIGHT);
		bool current_button_up = 		!HAL_GPIO_ReadPin(GPIO_BUTTON_UP, 		PIN_BUTTON_UP);
		bool current_button_down = 	!HAL_GPIO_ReadPin(GPIO_BUTTON_DOWN, 	PIN_BUTTON_DOWN);
		bool current_button_center = 	!HAL_GPIO_ReadPin(GPIO_BUTTON_CENTER, 	PIN_BUTTON_CENTER);

		if (current_button_left != previous_button_left) {
			*button_left_event = current_button_left ? BUTTON_PRESS_EVENT : BUTTON_RELEASE_EVENT;
			previous_button_left = current_button_left;
		}

		if (current_button_right != previous_button_right) {
			*button_right_event = current_button_right ? BUTTON_PRESS_EVENT : BUTTON_RELEASE_EVENT;
			previous_button_right = current_button_right;
		}

		if (current_button_up != previous_button_up) {
			*button_up_event = current_button_up ? BUTTON_PRESS_EVENT : BUTTON_RELEASE_EVENT;
			previous_button_up = current_button_up;
		}

		if (current_button_down != previous_button_down) {
			*button_down_event = current_button_down ? BUTTON_PRESS_EVENT : BUTTON_RELEASE_EVENT;
			previous_button_down = current_button_down;
		}

		if (current_button_center != previous_button_center) {
			*button_center_event = current_button_center ? BUTTON_PRESS_EVENT : BUTTON_RELEASE_EVENT;
			previous_button_center = current_button_center;
		}
	}
}

/*
	la fonction nous sert à détecter les différentes actions effectuées par l'utilisateur lorsque
    des boutons spécifiques ont été appuyés
*/
//user_actions_e BUTTON_get_action(void){
//	button_e left, right, up, down, center;
//	BUTTONS_update(&left, &right, &up, &down, &center);
//
////	if(center == BUTTON_PRESS_EVENT)
////		return ACTION_PLAY_PAUSE;
//	if(center == BUTTON_PRESS_EVENT)
//			return ACTION_OPEN_MENU;
//
//	if(left == BUTTON_PRESS_EVENT)
//		return ACTION_CLEAR_GRID;
//
//	if(right == BUTTON_PRESS_EVENT)
//		return ACTION_SAVE_GRID;
//
////	if(up == BUTTON_PRESS_EVENT)
////		return ACTION_OPEN_MENU;
//
//	if(down == BUTTON_PRESS_EVENT)
//		return ACTION_OPEN_SETTINGS;
//
//	return ACTION_NONE;
//}

