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

/* Includes ------------------------------------------------------------------*/

/* RTOS */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "timers.h"

/* CMSIS */
#include "stm32f4xx.h"

/* application */
#include "application/inc/Systemstate.h"
#include "application/inc/TaskController.h"
#include "application/inc/TaskMeasurement.h"
#include "application/inc/TaskMotor.h"
#include "application/inc/TaskCommunicationRX.h"
#include "application/inc/TaskCommunicationTX.h"

/* private typedef -----------------------------------------------------------*/
/* private define ------------------------------------------------------------*/
/* private macro -------------------------------------------------------------*/
/* private variables ---------------------------------------------------------*/
/* private function prototypes -----------------------------------------------*/
/* private functions ---------------------------------------------------------*/

/**
**===========================================================================
**
**  Abstract: main program
**
**===========================================================================
*/
int main(void)
{
	/* Ensure all priority bits are assigned as preemption priority bits. */
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);


	/* module initialisations */
	taskControllerInit();
	taskMeasurementInit();
	taskMotorInit();
	taskCommunicationRXInit();
	taskCommunicationTXInit();

	vTaskStartScheduler();

	/* Infinite loop */
	for(;;);

}
