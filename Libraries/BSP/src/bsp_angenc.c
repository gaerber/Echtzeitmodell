/**
 * \file        bsp_angdec.c
 * \brief
 * \date        2014-03-18
 * \version     0.1
 * \author		Kevin Gerber
 *
 * \addtogroup  bsp
 * @{
 *
 * \addtogroup  bsp_angdec
 * @{
 */

#include "bsp.h"
#include "bsp_angenc.h"

/*
 * EXTI_PortSourceGPIOC
 * EXTI_PinSource7
 * EXTI_Line7
 * EXTI9_5_IRQn
 */

void BSP_ARGENC_IRQ_Handler(void) {
    /* Incremental A GPIO rising edge interrupt */
	if(EXTI_GetITStatus(BSP_ANGENC_INCA.pin) != RESET) {
        EXTI_ClearITPendingBit(BSP_ANGENC_INCA.pin);
    }

	/* Incremental B GPIO rising edge interrupt */
    if(EXTI_GetITStatus(BSP_ANGENC_INCB.pin) != RESET) {
        EXTI_ClearITPendingBit(BSP_ANGENC_INCB.pin);
    }

    /* Incremental I (index/sync) GPIO rising edge interrupt */
    if(EXTI_GetITStatus(BSP_ANGENC_INCI.pin) != RESET) {
        EXTI_ClearITPendingBit(BSP_ANGENC_INCI.pin);
    }
}

/**
 * \brief	Initialize the tree GPIO interrupts for the incremental.
 */
void bsp_AngEncInit(void) {
	uint32_t i;
	EXTI_InitTypeDef EXTI_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	const bsp_gpioconf_t *port[3] = {&BSP_ANGENC_INCA, &BSP_ANGENC_INCB, &BSP_ANGENC_INCI};

	/* Enable SYSCFG clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

	/* Initialize the GPIOs as inputs. */
	for (i=0; i < 3; i++) {
		/* GPIO Peripheral clock enable */
		RCC_AHB1PeriphClockCmd(port[i]->periph, ENABLE);

		/* Configure GPIO pin as input with pull down */
		GPIO_InitStructure.GPIO_Pin = port[i]->pin;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
		GPIO_Init(port[i]->base, &GPIO_InitStructure);

		/* Interrupt initializing */

		/* Connect EXTI to the GPIO pin */
		SYSCFG_EXTILineConfig(BSP_GPIO_TO_EXTIPORT(port[i]->base),
				BSP_GPIO_PIN_TO_SOURCE(port[i]->pin));

		/* Configure EXTI line */
		EXTI_InitStructure.EXTI_Line = port[i]->pin;
		EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
		EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
		EXTI_InitStructure.EXTI_LineCmd = ENABLE;
		EXTI_Init(&EXTI_InitStructure);
	}

	/* Enable and set the Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = BSP_ARGENC_IRQ_CHANEL;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = BSP_ARGENC_IRQ_PRIORITY;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

/**
 * @}
 */

/**
 * @}
 */
