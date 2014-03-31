/**
 * \file        TaskMeasurement.c
 * \brief       calculate the light trigger
 * \date        2014-03-31
 * \version     0.0.1
 *
 * \addtogroup  measurement
 * \brief       controls the light flash trigger
 * @{
 */
/* Includes ------------------------------------------------------------------*/
/* RTOS */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

/* application */
#include "inc/Systemstate.h"
#include "inc/TaskMeasurement.h"


/* private typedef -----------------------------------------------------------*/
/* private define ------------------------------------------------------------*/
/* private macro -------------------------------------------------------------*/
/* private variables ---------------------------------------------------------*/
/* private function prototypes -----------------------------------------------*/
/* private functions ---------------------------------------------------------*/

/**
 *	\fn		taskMeasurement
 *	\brief	measurement task
 *
 *	\param[in]	pvParameters	necessary data for the controller task
 */
static void taskMeasurement(void* pvParameters)
{

	/* endless loop */
	for(;;)
	{

	}
}


/* public functions ----------------------------------------------------------*/

/**
 * \fn		taskMeasurementInit
 * \brief	creates the controller task
 */
void taskMeasurementInit()
{
	xTaskCreate(taskMeasurement, MEASUREMENT_TASK_NAME,
			MEASUREMENT_TASK_STACK_SIZE, NULL, MEASUREMENT_TASK_PRIORITY, NULL );
}
/**
 * @}
 */
