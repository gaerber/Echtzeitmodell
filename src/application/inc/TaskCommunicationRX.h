/**
 * \file        TaskCommunicationRX.h
 * \brief       RX part of the USART gatekeeper. Have the possibility to write direct in the TX ringbuffer for
 * 				echo functionality.
 * \date        2014-03-31
 * \version     0.0.1
 *
 * \addtogroup  gatekeeper
 * @{
 */

#ifndef TASKCOMMUNICATIONRX_H_
#define TASKCOMMUNICATIONRX_H_

/* exported typedef ----------------------------------------------------------*/
/* exported define -----------------------------------------------------------*/
#define COMMUNICATION_RX_TASK_PRIORITY			2
#define COMMUNICATION_RX_TASK_NAME				"Task Communication RX"
#define COMMUNICATION_RX_TASK_STACK_SIZE		configMINIMAL_STACK_SIZE
#define COMMUNICATION_RX_BUFFER_READOUT_TIME 	20 /*!< time in [ms] */


/* exported macro ------------------------------------------------------------*/


/* exported variables --------------------------------------------------------*/


/* exported function prototypes ----------------------------------------------*/
extern void taskCommunicationRXInit();

#endif /* TASKCOMMUNICATIONRX_H_ */

/**
 * @}
 */
