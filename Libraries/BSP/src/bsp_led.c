/**
 * \file        bsp_led.c
 * \brief       Board support package to use all the LEDs.
 * \date        2014-03-18
 * \version     0.1
 * \author		Kevin Gerber
 *
 * \note		Konfigurationen:
 * 				- RCC_AHB1Periph_GPIOI (Enegie Freigabe)
 * 				- GPIOI (Port)
 * 				- GPIO_Pin_6 (Pinnummer)
 *
 * \addtogroup  bsp
 * @{
 *
 * \addtogroup  bsp_led
 * @{
 */

#include "bsp.h"
#include "bsp_led.h"

/**
 * \brief	LED shadow register.
 */
//uint8_t led_shadow[BSP_LED_ELEMENTCTR] = {0};

/**
 * \brief	Initialize all LEDs on the board and sets them off as default.
 */
void bsp_LedInit(void) {
	bsp_led_t i;
	GPIO_InitTypeDef GPIO_InitStructure;

	/* Initialize all registred LED from the port array. */
	for (i=0; i < BSP_LED_ELEMENTCTR; i++) {
		/* GPIO Peripheral clock enable */
		RCC_AHB1PeriphClockCmd(port_led[i].periph, ENABLE);

		/* Get the default structure values */
		GPIO_StructInit(&GPIO_InitStructure);

		/* Configure GPIO in output pushpull mode */
		GPIO_InitStructure.GPIO_Pin = port_led[i].pin;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
		//GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
		//GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
		GPIO_Init(port_led[i].base, &GPIO_InitStructure);

		/* Set the LEDs off as the default state. */
		bsp_LedSetOff(i);
	}
}

/**
 * \brief	Turns a LED on.
 * \param	led The identification number of the LED, which turn on.
 */
void bsp_LedSetOn(bsp_led_t led) {
	assert(led < BSP_LED_ELEMENTCTR);

	/* Turns GPIO State high */
	GPIO_SetBits(port_led[led].base, port_led[led].pin);

	/* Update the shadow register */
	//led_shadow[led] = BSP_LED_ON;
}

/**
 * \brief	Turns a LED off.
 * \param	led The identification number of the LED, which turn off.
 */
void bsp_LedSetOff(bsp_led_t led) {
	assert(led < BSP_LED_ELEMENTCTR);

	/* Turns GPIO State high */
	GPIO_ResetBits(port_led[led].base, port_led[led].pin);

	/* Update the shadow register */
	//led_shadow[led] = BSP_LED_OFF;
}

/**
 * \brief	Toggles a LED.
 * \param	led The identification number of the LED, which toggle.
 */
void bsp_LedSetToggle(bsp_led_t led) {
	assert(led < BSP_LED_ELEMENTCTR);

	/* Turns GPIO State high */
	GPIO_ToggleBits(port_led[led].base, port_led[led].pin);

	/* Update the shadow register */
	//led_shadow[led] = BSP_LED_OFF;
}

/**
 * \brief		Get the state of an LED.
 * \param[in]	led		The identification number of the LED.
 *
 * \return		State of the LED.
 * \retval		BSP_LED_ON The LED is on.
 * \retval		BSP_LED_OFF The LED is off.
 */
//uint8_t bsp_LedGet__DEL_(bsp_led_t led) {
//	assert(led < BSP_LED_ELEMENTCTR);
//
//	/* Get the state from the shadow register */
//	return led_shadow[led];
//}

/**
 * @}
 */

/**
 * @}
 */
