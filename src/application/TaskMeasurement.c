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
QueueHandle_t gq_message;
/* private function prototypes -----------------------------------------------*/
/* private functions ---------------------------------------------------------*/

uint32_t convertChar2Pos(char c) {
	uint32_t pos;

	/* convert capital letters to small letters */
	if ( c >= 'A' && c <= 'Z') {
		c = c | 0x20;
	}

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
	uint32_t letter = 0;
	uint32_t letter_pos;
	char message[16];

	/* endless loop */
	for(;;)
	{
		/* Get new string if available (non blocked) */
		if (xQueueReceive(gq_message, message, 0) == pdTRUE) {
			letter = 0;
		}

		/* Restart the string */
		if (message[letter] == '\0') {
			letter = 0;
		}

		/* Short delay in front of each string */
		if (letter == 0) {
			bsp_AngEncPos(0);
			vTaskDelay(550);
		}

		/* Show the next char */
		letter_pos = convertChar2Pos(message[letter]);
		if (letter_pos > 0) {
			bsp_AngEncPos(letter_pos);
		}

		/* Show the letter for 500ms */
		vTaskDelay(500);

		/* Make a Short break with a empty screen */
		bsp_AngEncPos(0);
		vTaskDelay(50);

		/* Next letter */
		letter++;
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

	/* create queue per item */
	gq_message = xQueueCreate(MEASUREMENT_QUEUE_LENGTH, sizeof(char[16]));
}
/**
 * @}
 */
