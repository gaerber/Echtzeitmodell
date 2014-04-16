/**
 * \file        TaskMotor.c
 * \brief       controls the motor speed
 * \date        2014-03-31
 * \version     0.0.1
 *
 * \addtogroup  motor
 * \brief       motor control task
 * @{
 */
/* Includes ------------------------------------------------------------------*/
/* standard libraries */
#include <stdint.h>

/* RTOS */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

/* BSP */
#include "bsp_engine.h"
#include "bsp_angenc.h"

/* application */
#include "inc/Systemstate.h"
#include "inc/TaskMotor.h"

/* private typedef -----------------------------------------------------------*/
/* private define ------------------------------------------------------------*/
/* private macro -------------------------------------------------------------*/
/* private variables ---------------------------------------------------------*/
/* private function prototypes -----------------------------------------------*/
/* private functions ---------------------------------------------------------*/

/**
 *	\fn		taskMotor
 *	\brief	motor task
 *
 *	\param[in]	pvParameters	necessary data for the controller task
 */
static void taskMotor(void* pvParameters)
{
	/* local variables */
	portTickType last_flash_time;

	/* endless loop */
	for(;;)
	{
		bsp_EngineSpeedSet(g_systemstate.speed);
		vTaskDelayUntil(&last_flash_time, MOTOR_SPEED_REFRESH_TIME/portTICK_RATE_MS);
	}
}


/* public functions ----------------------------------------------------------*/

/**
 * \fn		taskMotorInit
 * \brief	creates the motor task
 */
void taskMotorInit()
{
	/* create task */
	xTaskCreate(taskMotor, MOTOR_TASK_NAME,
			MOTOR_TASK_STACK_SIZE, NULL, MOTOR_TASK_PRIORITY, NULL );

	/* init BSP modules */
	bsp_AngEncInit();
	bsp_EngineInit();

	bsp_EngineSpeedSet(g_systemstate.speed);
	bsp_EngineEnalble();
}

/**
 * @}
 */
