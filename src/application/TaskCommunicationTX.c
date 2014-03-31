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

/* application */
#include "inc/Systemstate.h"
#include "inc/TaskCommunicationTX.h"

/* private typedef -----------------------------------------------------------*/
/* private define ------------------------------------------------------------*/
/* private macro -------------------------------------------------------------*/
/* private variables ---------------------------------------------------------*/
SemaphoreHandle_t gm_tx_rinbuffer; /*!< mutex for tx ringbuffer access (for echo/
									data scheduling) */

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

	/* endless loop */
	for(;;)
	{

	}
}


/**
 * \fn		bsp_SerialIrqTxHandler
 * \brief	serial transmit IRQ callback function
 */
void bsp_SerialIrqTxHandler(void)
{

//	bsp_SerialSend(uart_string[char_ctr++]);

//	bspSerialTxIrqDisable();
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

	/* create mutex */
	gm_tx_rinbuffer = xSemaphoreCreateMutex();
}


/**
 * @}
 */
