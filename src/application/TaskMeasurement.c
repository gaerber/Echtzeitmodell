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
/* standard libraries */
#include <stdint.h>
#include <string.h>

/* RTOS */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

/* BSP */
#include "bsp_angenc.h"

/* application */
#include "inc/Systemstate.h"
#include "inc/TaskMeasurement.h"


/* private typedef -----------------------------------------------------------*/
/* private define ------------------------------------------------------------*/
/* private macro -------------------------------------------------------------*/
/* private variables ---------------------------------------------------------*/
QueueHandle_t gq_angle;
/* private function prototypes -----------------------------------------------*/
/* private functions ---------------------------------------------------------*/

uint32_t convertChar2Pos(char c) {
	uint32_t pos;

	if (c >= 'a' && c <= 'z') {
		pos = c - 'a' + 1;
	}
	else if (c == '!') {
		pos = 27;
	}
	else if (c == '?') {
		pos = 28;
	}
	else if (c == '-') {
		pos = 29;
	}
	else if (c == '.') {
		pos = 30;
	}
	else if (c == ',') {
		pos = 31;
	}
	else if (c == ':') {
		pos = 32;
	}
	else if (c == ';') {
		pos = 33;
	}
	else if (c == '\'') {
		pos = 34;
	}
	else if (c == ' ') {
		pos = 35;
	}
	else {
		pos = 0;
	}

	return pos;
}

/**
 *	\fn		taskMeasurement
 *	\brief	measurement task
 *
 *	\param[in]	pvParameters	necessary data for the controller task
 */
static void taskMeasurement(void* pvParameters)
{
	uint32_t letter;
	uint32_t letter_pos;
	char message[16];

	/* endless loop */
	for(;;)
	{
		/* Get the string */
		strcpy(message, g_systemstate.message);

		/* Show the hole string */
		letter = 0;
		while (message[letter] != '\0') {
			letter_pos = convertChar2Pos(message[letter]);
			if (letter_pos > 0) {
				bsp_AngEncPos(letter_pos);
			}

			letter++;

			vTaskDelay(500);
		}
	}
}


/* public functions ----------------------------------------------------------*/

/**
 * \fn		taskMeasurementInit
 * \brief	creates the controller task
 */
void taskMeasurementInit()
{
	/* create task */
	xTaskCreate(taskMeasurement, MEASUREMENT_TASK_NAME,
			MEASUREMENT_TASK_STACK_SIZE, NULL, MEASUREMENT_TASK_PRIORITY, NULL );

	/* create queue with 16bit space per item */
	gq_angle = xQueueCreate(MEASUREMENT_QUEUE_LENGTH, sizeof(uint16_t));

	/* init BSP modules */
	bsp_AngEncInit();
}
/**
 * @}
 */
