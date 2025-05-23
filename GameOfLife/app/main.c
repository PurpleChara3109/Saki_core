/**
  ******************************************************************************
  * @file    main.c
  * @author  Nirgal
  * @date    03-July-2019
  * @brief   Default main function.
  ******************************************************************************
*/
#include "gameoflife.h"
#include "stm32g4xx_hal.h"
#include "stm32g4_uart.h"
#include "stm32g4_sys.h"
#include "stm32g4_gpio.h"
#include "stm32g4_systick.h"
#include "TFT_ili9341/stm32g4_ili9341.h"  // ou le bon chemin


int main(void)
{
	//Initialisation de la couche logicielle HAL (Hardware Abstraction Layer)
	//Cette ligne doit rester la première étape de la fonction main().
	HAL_Init();

	//Initialisation de l'UART2 à la vitesse de 115200 bauds/secondes (92kbits/s) PA2 : Tx  | PA3 : Rx.
		//Attention, les pins PA2 et PA3 ne sont pas reliées jusqu'au connecteur de la Nucleo.
		//Ces broches sont redirigées vers la sonde de débogage, la liaison UART étant ensuite encapsul�e sur l'USB vers le PC de développement.
	BSP_UART_init(UART2_ID,115200);

	//"Indique que les printf sortent vers le périphérique UART2."
	BSP_SYS_set_std_usart(UART2_ID, UART2_ID, UART2_ID);

	//Initialisation du port de la led Verte
	BSP_GPIO_pin_config(LED_GREEN_GPIO, LED_GREEN_PIN, GPIO_MODE_OUTPUT_PP,GPIO_NOPULL,GPIO_SPEED_FREQ_HIGH, GPIO_MODE_OUTPUT_PP);

	//Initialisation de notre application

	GAMEOFLIFE_init();


	while(1)	//Boucle de tâche de fond
	{
		GAMEOFLIFE_process_main();
	}


}




