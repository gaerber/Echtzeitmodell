/**
 * \file        TaskCommunicationRX.c
 * \brief       RX part of the USART gatekeeper. Have the possibility to write direct in the TX ringbuffer for
 * 				echo functionality.
 * \date        2014-03-31
 * \version     0.0.1
 *
 * \addtogroup  gatekeeper
 * \brief       USART gatekeeper
 * @{
 */
/* Includes ------------------------------------------------------------------*/
/* standard libraries */
#include <stdint.h>

/* RTOS */
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "queue.h"

/* application */
#include "inc/Systemstate.h"
#include "inc/TaskCommunicationTX.h" /*!< dependency for ringbuffer mutex */
#include "inc/TaskController.h" /*!< dependency for message queue */
#include "inc/TaskCommunicationRX.h"

#include "circularbuffer.h"

/* private typedef -----------------------------------------------------------*/
/* private define ------------------------------------------------------------*/
/* private macro -------------------------------------------------------------*/
/* private variables ---------------------------------------------------------*/

/* private function prototypes -----------------------------------------------*/
/* private functions ---------------------------------------------------------*/
/**
 *	\fn		taskCommunicationRX
 *	\brief	communication RX task
 *
 *	\param[in]	pvParameters	necessary data for the controller task
 */
static void taskCommunicationRX(void* pvParameters)
{
	/* local variables */
	char command[64];
	uint8_t write_index;
	uint8_t command_complete;

	/* endless loop */
	for (;;) {

		/* Reset the command */
		write_index = 0;
		command_complete = 0;

		do {
			if (CircularBufferCharGet(&(command[write_index]))) {
				/* Character echo */
				if (g_systemstate.comm_echo) {
					/* Receive data */
					//xSemaphoreTake(gm_tx_rinbuffer, portMAX_DELAY);
					/* Puts the echo */
					while (!CircularBufferCharPut(command[write_index])) {
						vTaskDelay(10/portTICK_RATE_MS);
					}
				}

				/* Check the command end line */
				if (command[write_index] == '\r' || command[write_index] == '\n') {
					/* Command end detected */
					/* Terminate the string command */
					command[write_index] = '\0';
					command_complete = 1;
				}
				else {
					/* Increment index if more space for character */
					if (write_index < 64 - 1) {
						write_index++;
					}
					else {
						/* Command overflow detected */
						write_index = 0;
						xQueueSend(gq_tx_message, "#Command overflow", portMAX_DELAY);
					}
				}
			}
			else {
				/* No data in the circular buffer */
				vTaskDelay(10/portTICK_RATE_MS);
			}
		}
		while (command_complete == 0);

		/* Release the semaphore */
		//xSemaphoreGive(gm_tx_rinbuffer);

		/* Send the received command to the controller task */
		xQueueSend(gp_rx_message, command, portMAX_DELAY);
	}
}


/* public functions ----------------------------------------------------------*/

/**
 * \fn		taskCommunicationRXInit
 * \brief	creates the communication RX task
 */
void taskCommunicationRXInit()
{
	/* create task */
	xTaskCreate(taskCommunicationRX, COMMUNICATION_RX_TASK_NAME,
			COMMUNICATION_RX_TASK_STACK_SIZE, NULL, COMMUNICATION_RX_TASK_PRIORITY, NULL );

	/* Initialize the circular buffer and the hardware uart in interrupt mode */
	CircularBufferInit();
}

/**
 * @}
 */
