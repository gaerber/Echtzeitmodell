/**
 * \file        TaskController.h
 * \brief       processing the received messages from TaskCommunicationRX module and set the system state
 * \date        2014-03-31
 * \version     0.0.1
 *
 * \addtogroup  controller
 * @{
 */

#ifndef TASKCONTROLLER_H_
#define TASKCONTROLLER_H_

/* exported typedef ----------------------------------------------------------*/


/* exported define -----------------------------------------------------------*/
#define CONTROLLER_TASK_PRIORITY	2
#define CONTROLLER_TASK_NAME		"Task Controller"
#define CONTROLLER_TASK_STACK_SIZE	configMINIMAL_STACK_SIZE
#define CONTROLLER_QUEUE_LENGHT			10


/* exported macro ------------------------------------------------------------*/


/* exported variables --------------------------------------------------------*/
extern QueueHandle_t gq_message;


/* exported function prototypes ----------------------------------------------*/
extern void taskControllerInit();


#endif /* TASKCONTROLLER_H_ */

/**
 * @}
 */
