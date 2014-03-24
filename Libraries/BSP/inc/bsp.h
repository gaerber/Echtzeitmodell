/**
 * \file        bsp.h
 * \brief       Board support package.
 * \date        2014-03-18
 * \version     0.1
 * \author		Kevin Gerber
 *
 * \addtogroup  bsp
 * \brief		The board support package contains all medium layer functionality
 * 				to use the hardware module. The BSP is based on the standard peripheral
 * 				library from ST.
 * @{
 */

#ifndef BSP_H_
#define BSP_H_

#include "stm32f4xx.h"

/**
 * \brief	Assert makro.
 * \param	cond	Conditions, which must be true.
 * \todo	Define an assert function.
 */
#define assert(cond) //((cond)?(0): (fprintf (stderr, "assertion failed: \ %s, file %s, line %d \n",#cond,__FILE__,__LINE__), abort()))

/**
 * \brief	Structure of all important hardware configuration off each led.
 */
typedef struct {
	uint32_t periph;		/*!< RCC AHB peripheral of the port. */
	GPIO_TypeDef *base;		/*!< Port base address of the port. */
	uint16_t pin;			/*!< GPIO number of the LED. */
	uint8_t af;				/*!< GPIO alternate function */
	uint8_t fillup;
} bsp_gpioconf_t;


/**
 * \brief	Get the pin source from the pin number.
 * \author	Tobias Rueetschi
 */
#define BSP_GPIO_PIN_TO_SOURCE(GPIO_PIN)	(								\
		((GPIO_PIN) & GPIO_Pin_0) ? GPIO_PinSource0 :	/* bit 0 is set? */	\
		((GPIO_PIN) & GPIO_Pin_1) ? GPIO_PinSource1 :	/* bit 1 is set? */	\
		((GPIO_PIN) & GPIO_Pin_2) ? GPIO_PinSource2 :	/* bit 2 is set? */	\
		((GPIO_PIN) & GPIO_Pin_3) ? GPIO_PinSource3 :	/* bit 3 is set? */	\
		((GPIO_PIN) & GPIO_Pin_4) ? GPIO_PinSource4 :	/* bit 4 is set? */	\
		((GPIO_PIN) & GPIO_Pin_5) ? GPIO_PinSource5 :	/* bit 5 is set? */	\
		((GPIO_PIN) & GPIO_Pin_6) ? GPIO_PinSource6 :	/* bit 6 is set? */	\
		((GPIO_PIN) & GPIO_Pin_7) ? GPIO_PinSource7 :	/* bit 7 is set? */	\
		((GPIO_PIN) & GPIO_Pin_8) ? GPIO_PinSource8 :	/* bit 8 is set? */	\
		((GPIO_PIN) & GPIO_Pin_9) ? GPIO_PinSource9 :	/* bit 9 is set? */	\
		((GPIO_PIN) & GPIO_Pin_10) ? GPIO_PinSource10 :	/* bit 10 is set? */\
		((GPIO_PIN) & GPIO_Pin_11) ? GPIO_PinSource11 :	/* bit 11 is set? */\
		((GPIO_PIN) & GPIO_Pin_12) ? GPIO_PinSource12 :	/* bit 12 is set? */\
		((GPIO_PIN) & GPIO_Pin_13) ? GPIO_PinSource13 :	/* bit 13 is set? */\
		((GPIO_PIN) & GPIO_Pin_14) ? GPIO_PinSource14 :	/* bit 14 is set? */\
		((GPIO_PIN) & GPIO_Pin_15) ? GPIO_PinSource15 :	/* bit 15 is set? */\
		-1)												/* no bits are set */


/**
 * \brief	Get the port source from the port number.
 * \author	Tobias Rueetschi
 */
#define BSP_GPIO_TO_EXTIPORT(GPIO_PORT)	(								\
		(GPIO_PORT == GPIOA) ? EXTI_PortSourceGPIOA :	/* Port A? */		\
		(GPIO_PORT == GPIOB) ? EXTI_PortSourceGPIOB :	/* Port B? */		\
		(GPIO_PORT == GPIOC) ? EXTI_PortSourceGPIOC :	/* Port C? */		\
		(GPIO_PORT == GPIOD) ? EXTI_PortSourceGPIOD :	/* Port D? */		\
		(GPIO_PORT == GPIOE) ? EXTI_PortSourceGPIOE :	/* Port E? */		\
		(GPIO_PORT == GPIOF) ? EXTI_PortSourceGPIOF :	/* Port F? */		\
		(GPIO_PORT == GPIOG) ? EXTI_PortSourceGPIOG :	/* Port G? */		\
		(GPIO_PORT == GPIOH) ? EXTI_PortSourceGPIOH :	/* Port H? */		\
		(GPIO_PORT == GPIOI) ? EXTI_PortSourceGPIOI :	/* Port I? */		\
		-1)												/* no port is set */


/**
 * \brief	Get the pin EXTI source from the pin number.
 * \author	Tobias Rueetschi
 */
#define BSP_GPIO_TO_EXTIPIN(GPIO_PIN)	(									\
		((GPIO_PIN) & GPIO_Pin_0) ? EXTI_PinSource0 :	/* bit 0 is set? */	\
		((GPIO_PIN) & GPIO_Pin_1) ? EXTI_PinSource1 :	/* bit 1 is set? */	\
		((GPIO_PIN) & GPIO_Pin_2) ? EXTI_PinSource2 :	/* bit 2 is set? */	\
		((GPIO_PIN) & GPIO_Pin_3) ? EXTI_PinSource3 :	/* bit 3 is set? */	\
		((GPIO_PIN) & GPIO_Pin_4) ? EXTI_PinSource4 :	/* bit 4 is set? */	\
		((GPIO_PIN) & GPIO_Pin_5) ? EXTI_PinSource5 :	/* bit 5 is set? */	\
		((GPIO_PIN) & GPIO_Pin_6) ? EXTI_PinSource6 :	/* bit 6 is set? */	\
		((GPIO_PIN) & GPIO_Pin_7) ? EXTI_PinSource7 :	/* bit 7 is set? */	\
		((GPIO_PIN) & GPIO_Pin_8) ? EXTI_PinSource8 :	/* bit 8 is set? */	\
		((GPIO_PIN) & GPIO_Pin_9) ? EXTI_PinSource9 :	/* bit 9 is set? */	\
		((GPIO_PIN) & GPIO_Pin_10) ? EXTI_PinSource10 :	/* bit 10 is set? */\
		((GPIO_PIN) & GPIO_Pin_11) ? EXTI_PinSource11 :	/* bit 11 is set? */\
		((GPIO_PIN) & GPIO_Pin_12) ? EXTI_PinSource12 :	/* bit 12 is set? */\
		((GPIO_PIN) & GPIO_Pin_13) ? EXTI_PinSource13 :	/* bit 13 is set? */\
		((GPIO_PIN) & GPIO_Pin_14) ? EXTI_PinSource14 :	/* bit 14 is set? */\
		((GPIO_PIN) & GPIO_Pin_15) ? EXTI_PinSource15 :	/* bit 15 is set? */\
		-1)												/* no bits are set */


#endif /* BSP_H_ */

/**
 * @}
 */
