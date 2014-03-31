/**
 * \file        main.c
 * \brief       initialisation of the different system modules
 *
 * \mainpage    Echtzeitmodell
 * \authors     K. Gerber, S. Grossenbacher
 * \date        2014-03-31
 * \version     0.0.1
 *
 * \addtogroup  main
 * \brief       main function
 * @{
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

/* application */
#include "application/inc/Systemstate.h"
#include "application/inc/TaskController.h"
#include "application/inc/TaskMeasurement.h"

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
//	volatile uint32_t i;
//	uint32_t brightness = 0;

	/* Ensure all priority bits are assigned as preemption priority bits. */
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);

//	bsp_LedInit();
//	bsp_AngEncInit();
//	bsp_EngineInit();
//
//	bsp_SerialInit();
//
//	bsp_EngineEnalble();
//	bsp_EngineSpeedSet(brightness);


	/* Send over UART (only pend interrupt to start transmission) */
//	bspSerialTxIrqEnable();



	/* module initialisations */
	taskControllerInit();
	taskMeasurementInit();

	vTaskStartScheduler();

	/* Infinite loop */
	for(;;);

}
