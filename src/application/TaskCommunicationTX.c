/**
 * \file        TaskCommunicationTX.c
 * \brief       TX part of the USART gatekeeper
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
#include "inc/TaskCommunicationTX.h"

#include "circularbuffer.h"

/* private typedef -----------------------------------------------------------*/
/* private define ------------------------------------------------------------*/
/* private macro -------------------------------------------------------------*/
/* private variables ---------------------------------------------------------*/
SemaphoreHandle_t gm_tx_rinbuffer; /*!< mutex for tx ringbuffer access (for echo/
									data scheduling) */
QueueHandle_t gq_tx_message;

/* private function prototypes -----------------------------------------------*/
/* private functions ---------------------------------------------------------*/

/**
 *	\fn		taskCommunicationTX
 *	\brief	communication TX task
 *
 *	\param[in]	pvParameters	necessary data for the controller task
 */
static void taskCommunicationTX(void* pvParameters)
{
	char output[64];
	char *ptr;

	/* endless loop */
	for (;;) {
		/* Get the message, which has to be sent */
		if (xQueueReceive(gq_tx_message, output, portMAX_DELAY) == pdTRUE) {
			/* Gets the semaphore to write into the circular buffer */
			xSemaphoreTake(gm_tx_rinbuffer, portMAX_DELAY);

			/* Send the string to the TX output buffer */
			ptr = output;
			while (*ptr != '\0') {
				while (!CircularBufferCharPut(*ptr)) {
					/* No space available in the circular buffer */
					vTaskDelay(10/portTICK_RATE_MS);
				}
			}

			/* Release the semaphore */
			xSemaphoreGive(gm_tx_rinbuffer);
		}
	}
}


/* public functions ----------------------------------------------------------*/

/**
 * \fn		taskCommunicationTXInit
 * \brief	creates the communication TX task
 */
void taskCommunicationTXInit()
{
	/* create task */
	xTaskCreate(taskCommunicationTX, COMMUNICATION_TX_TASK_NAME,
			COMMUNICATION_TX_TASK_STACK_SIZE, NULL, COMMUNICATION_TX_TASK_PRIORITY, NULL );

	/* create queue with 64 char space per item */
	gq_tx_message = xQueueCreate(COMMUNICATION_TX_QUEUE_LENGHT, sizeof(char[64]));

	/* create mutex */
	gm_tx_rinbuffer = xSemaphoreCreateMutex();
}


/**
 * @}
 */
