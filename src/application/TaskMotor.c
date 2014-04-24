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
QueueHandle_t gq_motor_speed;

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
	uint8_t new_speed;

	/* endless loop */
	for(;;)
	{
		if (xQueueReceive(gq_motor_speed, &new_speed, portMAX_DELAY) == pdTRUE) {
			bsp_EngineSpeedSet(new_speed);
		}
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

	/* create queue with engine speed */
	gq_motor_speed = xQueueCreate(MOTOR_SPEED_QUEUE_LENGHT, sizeof(uint8_t));

	/* init BSP modules */
	bsp_AngEncInit();
	bsp_EngineInit();

	bsp_EngineSpeedSet(g_systemstate.speed);
	bsp_EngineEnalble();
}

/**
 * @}
 */
