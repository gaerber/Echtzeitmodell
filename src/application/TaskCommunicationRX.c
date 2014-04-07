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
#include "inc/TaskCommunicationRX.h"

#include "bsp_serial.h"

/* private typedef -----------------------------------------------------------*/
/* private define ------------------------------------------------------------*/
/* private macro -------------------------------------------------------------*/
/* private variables ---------------------------------------------------------*/
QueueHandle_t gq_message;
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
	portTickType last_flash_time;

	/* endless loop */
	for(;;)
	{
		vTaskDelayUntil(&last_flash_time, COMMUNICATION_RX_BUFFER_READOUT_TIME/portTICK_RATE_MS);
	}
}

/**
 * \fn		bsp_SerialIrqRxHandler
 * \brief	serial receive IRQ callback function
 */
void bsp_SerialIrqRxHandler(void)
{
//	uint16_t recv;
//	bsp_SerialReceive(&recv);
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

	/* create queue with 16 char space per item */
	gq_message = xQueueCreate(COMMUNICATION_RX_QUEUE_LENGHT, sizeof(char[16]));

	/* init modules */
	bsp_SerialInit();
	bspSerialTxIrqEnable();
}

/**
 * @}
 */
