/**
 * \file        bsp_angenc.h
 * \brief       Angel encoder. Supports the GPIO interrupts of the incremental of
 * 				the rotation axe of the engine.
 * \date        2014-03-18
 * \version     0.1
 * \author		Kevin Gerber
 *
 * \addtogroup  bsp
 * @{
 *
 * \addtogroup  bsp_angdec
 * \brief		Angel encoder. Supports the GPIO interrupts of the incremental of
 * 				the rotation axe of the engine.
 * \bug			Works only if all EXIT line are on the same interrupt!
 * @{
 */

#ifndef BSP_ANGENC_H_
#define BSP_ANGENC_H_

#include "bsp.h"

/** Hardware label from the interrupt input of the incremental A (Carme IO2 IN1) */
static const bsp_gpioconf_t BSP_ANGENC_INCA = {RCC_AHB1Periph_GPIOG, GPIOG, GPIO_Pin_6};
//static const bsp_gpioconf_t BSP_ANGENC_INCA = {RCC_AHB1Periph_GPIOC, GPIOC, GPIO_Pin_7};

/** Hardware label from the interrupt input of the incremental B (Carme IO2 IN2) */
static const bsp_gpioconf_t BSP_ANGENC_INCB = {RCC_AHB1Periph_GPIOG, GPIOG, GPIO_Pin_7};

/** Hardware label from the interrupt input of the incremental index (Carme IO2 IN0) */
static const bsp_gpioconf_t BSP_ANGENC_INCI = {RCC_AHB1Periph_GPIOG, GPIOG, GPIO_Pin_8};

/* Interrupt settings */
#define BSP_ARGENC_IRQ_CHANEL	EXTI9_5_IRQn		/*!< NVIC UART interrupt */
#define BSP_ARGENC_IRQ_PRIORITY	8					/*!< NVIC UART interrupt priority */
#define BSP_ARGENC_IRQ_Handler	EXTI9_5_IRQHandler	/*!< NVIC interrupt handler */

/* Function prototypes */
extern void bsp_AngEncInit(void);

#endif /* BSP_ANGENC_H_ */

/**
 * @}
 */

/**
 * @}
 */
