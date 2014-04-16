/**
 * \file        bsp_engine.c
 * \brief       Supports the PWM output to control the engine.
 * \date        2014-03-18
 * \version     0.1
 * \author		Kevin Gerber
 *
 * \addtogroup  bsp
 * @{
 *
 * \addtogroup  bsp_engine
 * @{
 */

#include "bsp_engine.h"

/**
 * \brief	Initialize the dc-engine. It is used one PWM channel to set the engine speed.
 */
void bsp_EngineInit(void) {
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;

	uint16_t PrescalerValue;

	/* Initialize all GPIOs in their function */
	bsg_GpioInit(&BSP_ENGINE_PORT);

	/* TIM clock enable */
	RCC_APB1PeriphClockCmd(BSP_ENGINE_TIMER_PORT_PERIPH, ENABLE);

	/* Compute the prescaler value */
	PrescalerValue = (uint16_t) ((SystemCoreClock /2) / BSP_ENGINE_PWM_FREQ) - 1;

	/* Time base configuration */
	TIM_TimeBaseStructure.TIM_Period = BSP_ENGINE_PWM_PERIOD;
	TIM_TimeBaseStructure.TIM_Prescaler = PrescalerValue;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(BSP_ENGINE_TIMER_PORT_BASE, &TIM_TimeBaseStructure);

	/* PWM Mode configuration */
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = 0;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	/* Check the PWM port */
	switch (BSP_ENGINE_PWM_PORT_CHANEL) {
	case CHANEL1:
		/* PWM Mode configuration: Channel1 */
		TIM_OC1Init(BSP_ENGINE_TIMER_PORT_BASE, &TIM_OCInitStructure);
		TIM_OC1PreloadConfig(BSP_ENGINE_TIMER_PORT_BASE, TIM_OCPreload_Enable);
		break;

	case CHANEL2:
		/* PWM Mode configuration: Channel2 */
		TIM_OC2Init(BSP_ENGINE_TIMER_PORT_BASE, &TIM_OCInitStructure);
		TIM_OC2PreloadConfig(BSP_ENGINE_TIMER_PORT_BASE, TIM_OCPreload_Enable);
		break;

	case CHANEL3:
		/* PWM Mode configuration: Channel3 */
		TIM_OC3Init(BSP_ENGINE_TIMER_PORT_BASE, &TIM_OCInitStructure);
		TIM_OC3PreloadConfig(BSP_ENGINE_TIMER_PORT_BASE, TIM_OCPreload_Enable);
		break;

	case CHANEL4:
		/* PWM Mode configuration: Channel4 */
		TIM_OC4Init(BSP_ENGINE_TIMER_PORT_BASE, &TIM_OCInitStructure);
		TIM_OC4PreloadConfig(BSP_ENGINE_TIMER_PORT_BASE, TIM_OCPreload_Enable);
		break;

	default:
		assert(BSP_ENGINE_PWM_PORT);
		break;
	}

	/* Enables  TIMx peripheral preload register on ARR */
	TIM_ARRPreloadConfig(BSP_ENGINE_TIMER_PORT_BASE, ENABLE);
}

/**
 * \brief	Enable the engine. It will turn with the configured speed.
 */
void bsp_EngineEnalble(void) {
	/* Enable counter */
	TIM_Cmd(BSP_ENGINE_TIMER_PORT_BASE, ENABLE);
}

/**
 * \brief	Disable the engine. It will be stopped.
 */
void bsp_EngineDisable(void) {
	/* Enable counter */
	TIM_Cmd(BSP_ENGINE_TIMER_PORT_BASE, DISABLE);
}

/**
 * \brief	Changes the engine speed by update the duty cycle of the PWM output.
 * \note	To get more performance the standard peripheral library is not used.
 * \param	speed	New speed of the engine. Equivalent to the duty cycle of the PWM.
 * 			It must be smaller than the period register, which is configured in
 * 			\var BSP_ENGINE_PWM_PERIOD.
 * \bug		CCRx Register: x is the channel number!
 */
void bsp_EngineSpeedSet(uint32_t speed) {
	/* Set the Capture Compare Register value */
	BSP_ENGINE_TIMER_PORT_BASE->CCR4 = (speed % BSP_ENGINE_PWM_PERIOD);
}

/**
 * \todo	Check if the CCR register is readable!
 */
void bsp_EngineSpeedGet(uint32_t *speed) {
	*speed = BSP_ENGINE_TIMER_PORT_BASE->CCR4;
}

/**
 * @}
 */

/**
 * @}
 */
