/**
 * \file        TaskCommunicationTX.h
 * \brief       TX part of the USART gatekeeper
 * \date        2014-03-31
 * \version     0.0.1
 *
 * \addtogroup  gatekeeper
 * \brief       USART gatekeeper
 * @{
 */

#ifndef TASKCOMMUNICATIONTX_H_
#define TASKCOMMUNICATIONTX_H_

#include "semphr.h"

/* exported typedef ----------------------------------------------------------*/
/* exported define -----------------------------------------------------------*/
#define COMMUNICATION_TX_TASK_PRIORITY		2
#define COMMUNICATION_TX_TASK_NAME			"Task Communication TX"
#define COMMUNICATION_TX_TASK_STACK_SIZE	configMINIMAL_STACK_SIZE
#define COMMUNICATION_TX_QUEUE_LENGHT		10


/* exported macro ------------------------------------------------------------*/


/* exported variables --------------------------------------------------------*/
extern SemaphoreHandle_t gm_tx_rinbuffer;
extern QueueHandle_t gq_tx_message;

/* exported function prototypes ----------------------------------------------*/
extern void taskCommunicationTXInit();



#endif /* TASKCOMMUNICATIONTX_H_ */

/**
 * @}
 */
