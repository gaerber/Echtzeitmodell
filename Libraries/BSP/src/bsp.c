/**
 * \file        bsp.c
 * \brief       Board support package main functions.
 * \date        2014-03-18
 * \version     0.1
 * \author		Kevin Gerber
 *
 * \addtogroup  bsp
 * @{
 */

#include "bsp.h"

/**
 * \brief	Initialize a GPIO in standard or alternate function mode.
 * \param	port Pointer to the GPIO configuration structure.
 */
void bsg_GpioInit(const bsp_gpioconf_t *port) {
	GPIO_InitTypeDef GPIO_InitStructure;

	/* GPIO Peripheral clock enable */
	RCC_AHB1PeriphClockCmd(port->periph, ENABLE);

	/* Configure GPIO */
	GPIO_InitStructure.GPIO_Pin = port->pin;
	GPIO_InitStructure.GPIO_Mode = port->mode;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = port->pupd;
	GPIO_Init(port->base, &GPIO_InitStructure);

	/* Sets the alternate function if necessary */
	if (port->mode == GPIO_Mode_AF) {
		/* Connect the alternate function to the pin */
		GPIO_PinAFConfig(port->base, BSP_GPIO_PIN_TO_SOURCE(port->pin), port->af);
	}
}
