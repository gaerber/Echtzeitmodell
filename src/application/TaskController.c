/**
 * \file        TaskController.c
 * \brief       processing the received messages from TaskCommunicationRX module and set the system state
 * \date        2014-03-31
 * \version     0.0.1
 *
 * \addtogroup  controller
 * \brief       controls the system flow
 * @{
 */
/* Includes ------------------------------------------------------------------*/
/* RTOS */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

/* application */
#include "inc/Systemstate.h"
#include "inc/TaskController.h"

/* private typedef -----------------------------------------------------------*/
/* private define ------------------------------------------------------------*/
/* private macro -------------------------------------------------------------*/
/* private variables ---------------------------------------------------------*/


/* private function prototypes -----------------------------------------------*/



/* private functions ---------------------------------------------------------*/
/**
 *	\fn		taskController
 *	\brief	controller task
 *
 *	\param[in]	pvParameters	necessary data for the controller task
 */
static void taskController(void* pvParameters)
{

	/* endless loop */
	for(;;)
	{

	}
}


/* public functions ----------------------------------------------------------*/

/**
 * \fn		taskControllerInit
 * \brief	creates the controller task
 */
void taskControllerInit()
{
	xTaskCreate(taskController, CONTROLLER_TASK_NAME,
			CONTROLLER_TASK_STACK_SIZE, NULL, CONTROLLER_TASK_PRIORITY, NULL );
}

/**
 * @}
 */
