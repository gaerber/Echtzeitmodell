/**
 * \file        bsp_angdec.c
 * \brief
 * \date        2014-03-18
 * \version     0.2
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

#include "bsp_led.h"

/*
 * EXTI_PortSourceGPIOC
 * EXTI_PinSource7
 * EXTI_Line7
 * EXTI9_5_IRQn
 */

static uint32_t g_angle;
static uint32_t g_angle_capture = 0;

void BSP_ARGENC_IRQ_Handler(void) {
    /* Incremental A GPIO rising edge interrupt */
	if(EXTI_GetITStatus(BSP_ANGENC_INCA.pin) != RESET) {
        EXTI_ClearITPendingBit(BSP_ANGENC_INCA.pin);
        g_angle++;
        if (g_angle == g_angle_capture) {
        	/* Trigger the strobo */
            bsp_LedSetOn(BSP_LED_STROBE);
            bsp_LedSetOff(BSP_LED_STROBE);
        }
    }

	/* Incremental B GPIO rising edge interrupt */
    if(EXTI_GetITStatus(BSP_ANGENC_INCB.pin) != RESET) {
        EXTI_ClearITPendingBit(BSP_ANGENC_INCB.pin);
    }

    /* Incremental I (index/sync) GPIO rising edge interrupt */
    if(EXTI_GetITStatus(BSP_ANGENC_INCI.pin) != RESET) {
    	EXTI_ClearITPendingBit(BSP_ANGENC_INCI.pin);
    	g_angle = 0;
    }
}

/**
 * \brief	Initialize the tree GPIO interrupts for the incremental.
 */
void bsp_AngEncInit(void) {
	EXTI_InitTypeDef EXTI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	/* Initialize all GPIOs in their function */
	bsg_GpioInit(&BSP_ANGENC_INCA);
	bsg_GpioInit(&BSP_ANGENC_INCB);
	bsg_GpioInit(&BSP_ANGENC_INCI);

	/* Enable SYSCFG clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

	/* Connect EXTI to the GPIO pin */
	SYSCFG_EXTILineConfig(BSP_GPIO_TO_EXTIPORT(BSP_ANGENC_INCA.base),
			BSP_GPIO_PIN_TO_SOURCE(BSP_ANGENC_INCA.pin));
	SYSCFG_EXTILineConfig(BSP_GPIO_TO_EXTIPORT(BSP_ANGENC_INCB.base),
			BSP_GPIO_PIN_TO_SOURCE(BSP_ANGENC_INCB.pin));
	SYSCFG_EXTILineConfig(BSP_GPIO_TO_EXTIPORT(BSP_ANGENC_INCI.base),
			BSP_GPIO_PIN_TO_SOURCE(BSP_ANGENC_INCI.pin));

	/* Configure EXTI line */
	EXTI_InitStructure.EXTI_Line = BSP_ANGENC_INCA.pin | BSP_ANGENC_INCB.pin | BSP_ANGENC_INCI.pin;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);

	/* Enable and set the Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = BSP_ARGENC_IRQ_CHANEL;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = BSP_ARGENC_IRQ_PRIORITY;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

/**
 * \brief	Set the next interrupt angle position.
 * \param	pos Counted position of the angle.
 */
void bsp_AngEncPos(uint32_t pos) {
	g_angle_capture = pos;
}

/**
 * @}
 */

/**
 * @}
 */
