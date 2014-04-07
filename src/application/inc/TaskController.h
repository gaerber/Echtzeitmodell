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
#define CONTROLLER_QUEUE_LENGHT		10

/* messages */
#define MESSAGES_MAX_LENGTH 16

/* error codes */
#define ERROR_ACK_CODE 0
#define ERROR_ACK_MSG " ack"
#define ERROR_ACK ERROR_ACK_CODE ERROR_ACK_MSG
#define ERROR_UNKNOW_CMD_CODE 11
#define ERROR_UNKNOW_CMD_MSG " unknown command"
#define ERROR_UNKNOW_CMD ERROR_UNKNOW_CMD_CODE ERROR_UNKNOW_CMD_MSG
#define ERROR_TOO_FEW_ARG_CODE "21"
#define ERROR_TOO_FEW_ARG_MSG " too few arguments"
#define ERROR_TOO_FEW_ARG ERROR_TOO_FEW_ARG_CODE ERROR_TOO_FEW_ARG_MSG
#define ERROR_FAULT_ARG_TYP_CODE "22"
#define ERROR_FAULT_ARG_TYP_MSG " fault arguments type"
#define ERROR_ARG_OUT_OF_BND_CODE "31"
#define ERROR_ARG_OUT_OF_BND_MSG " argument out of bound"
#define ERROR_ARG_OUT_OF_BND ERROR_ARG_OUT_OF_BND_CODE ERROR_ARG_OUT_OF_BND_MSG
#define ERROR_LINE_OVFL_CODE "41"
#define ERROR_LINE_OVFL_MSG " line overflow"
#define ERROR_LINE_OVFL ERROR_LINE_OVFL_CODE ERROR_LINE_OVFL_MSG


/* exported macro ------------------------------------------------------------*/


/* exported variables --------------------------------------------------------*/
extern QueueHandle_t gp_rx_message;


/* exported function prototypes ----------------------------------------------*/
extern void taskControllerInit();


#endif /* TASKCONTROLLER_H_ */

/**
 * @}
 */
