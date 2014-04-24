/**
 * \file        bsp_led.c
 * \brief       Board support package to use all the LEDs.
 * \date        2014-03-18
 * \version     0.2
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
	bsp_led_t i_led;

	/* Initialize all registred LED from the port array. */
	for (i_led=0; i_led < BSP_LED_ELEMENTCTR; i_led++) {
		/* Initialize all GPIOs in their function */
		bsg_GpioInit(&(BSP_LED_PORTS[i_led]));

		/* Set the LEDs off as the default state. */
		bsp_LedSetOff(i_led);
	}
}

/**
 * \brief	Turns a LED on.
 * \param	led The identification number of the LED, which turn on.
 */
void bsp_LedSetOn(bsp_led_t led) {
	assert(led < BSP_LED_ELEMENTCTR);

	/* Turns GPIO State high */
	GPIO_SetBits(BSP_LED_PORTS[led].base, BSP_LED_PORTS[led].pin);

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
	GPIO_ResetBits(BSP_LED_PORTS[led].base, BSP_LED_PORTS[led].pin);

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
	GPIO_ToggleBits(BSP_LED_PORTS[led].base, BSP_LED_PORTS[led].pin);

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
