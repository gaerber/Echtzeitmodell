/**
 * \file        bsp_engine.h
 * \brief       Supports the PWM output to control the engine.
 * \date        2014-03-18
 * \version     0.1
 * \author		Kevin Gerber
 *
 * \addtogroup  bsp
 * @{
 *
 * \addtogroup  bsp_engine
 * \brief		????
 * @{
 */

#ifndef BSP_ENGINE_H_
#define BSP_ENGINE_H_

#include "bsp.h"

/**
 * \brief	All available PWM controller and channel.
 */
typedef enum {
	CHANEL1,		/*!< PWM Channel 1 */
	CHANEL2,		/*!< PWM Channel 2 */
	CHANEL3,		/*!< PWM Channel 3 */
	CHANEL4			/*!< PWM Channel 4 */
} bsp_pwmport_t;

/** PWM frequency */
#define BSP_ENGINE_PWM_FREQ				21000000
/** Period register of the PWM. */
#define BSP_ENGINE_PWM_PERIOD			100

/** RCC AHB peripheral of the timer port. */
#define BSP_ENGINE_TIMER_PORT_PERIPH	RCC_APB1Periph_TIM5
/** Port base address of the timer port */
#define BSP_ENGINE_TIMER_PORT_BASE		TIM5
/** Used PWM type */
#define BSP_ENGINE_PWM_PORT_CHANEL		CHANEL1

/** Hardware label of the PWM output pin, which is connected to the engine. */
static const bsp_gpioconf_t BSP_ENGINE_PORT = {
		RCC_AHB1Periph_GPIOA, GPIOA, GPIO_Pin_0, GPIO_AF_TIM5
};

/* Function prototypes */
extern void bsp_EngineInit(void);
extern void bsp_EngineEnalble(void);
extern void bsp_EngineDisable(void);
extern void bsp_EngineSpeedSet(uint32_t speed);
extern void bsp_EngineSpeedGet(uint32_t *speed);

#endif /* BSP_ENGINE_H_ */

/**
 * @}
 */

/**
 * @}
 */
