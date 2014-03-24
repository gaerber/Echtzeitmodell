/**
*****************************************************************************
**
**  File        : main.c
**
**  Abstract    : main function.
**
**  Functions   : main
**
**  Environment : Atollic TrueSTUDIO(R)
**                STMicroelectronics STM32F4xx Standard Peripherals Library
**
**  Distribution: The file is distributed “as is,” without any warranty
**                of any kind.
**
**  (c)Copyright Atollic AB.
**  You may use this file as-is or modify it according to the needs of your
**  project. This file may only be built (assembled or compiled and linked)
**  using the Atollic TrueSTUDIO(R) product. The use of this file together
**  with other tools than Atollic TrueSTUDIO(R) is not permitted.
**
*****************************************************************************
*/

/* Includes */
/* RTOS */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "timers.h"

/* CMSIS */
#include "stm32f4xx.h"

/* BSP */
#include "bsp.h"
#include "bsp_led.h"
#include "bsp_angenc.h"
#include "bsp_engine.h"
#include "bsp_serial.h"

/* Private macro */
/* Private variables */
/* Private function prototypes */
/* Private functions */

volatile uint32_t char_ctr = 0;
static const uint32_t size_of_char = 21;
static const uint8_t uart_string[] = "Herzlich Willkommen\r\n";

void bsp_SerialIrqTxHandler(void) {
	if (char_ctr < size_of_char)
		bsp_SerialSend(uart_string[char_ctr++]);
	else
		bspSerialTxIrqDisable();
}

void bsp_SerialIrqRxHandler(void) {
	uint16_t recv;
	bsp_SerialReceive(&recv);
}

/**
**===========================================================================
**
**  Abstract: main program
**
**===========================================================================
*/
int main(void) {
	volatile uint32_t i;
	uint32_t brightness = 0;

	/* Ensure all priority bits are assigned as preemption priority bits. */
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);

	bsp_LedInit();
	bsp_AngEncInit();
	bsp_EngineInit();

	bsp_SerialInit();

	bsp_EngineEnalble();
	bsp_EngineSpeedSet(brightness);

	__enable_irq();

	for (i=0; i<0x3FFFFF; i++);
	//EXTI_GenerateSWInterrupt(EXTI_Line7);

	/* Send over UART */
	bspSerialTxIrqEnable();
	//bsp_SerialSend(uart_string[char_ctr++]);

	/* Infinite loop */
	while (1) {
		/* Set */
		bsp_LedSetOn(BSP_LED_GREEN);
		bsp_LedSetOn(BSP_LED_RED);
		for (i=0; i<0x3FFFFF; i++);
		/* Reset */
		bsp_LedSetOff(BSP_LED_GREEN);
		bsp_LedSetOff(BSP_LED_RED);
		for (i=0; i<0x3FFFFF; i++);

		bsp_EngineSpeedSet(brightness);
		brightness += 10;
	}
}
