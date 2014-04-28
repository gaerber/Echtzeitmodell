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
#define MESSAGES_MAX_LENGTH 64

/* error codes */
#define ERROR_ACK_CODE 0
#define ERROR_ACK_STR_CODE "00"
#define ERROR_ACK_MSG "ack"

#define ERROR_UNKNOW_CMD_CODE 11
#define ERROR_UNKNOW_CMD_STR_CODE "11"
#define ERROR_UNKNOW_CMD_MSG "unknown command"
#define ERROR_TOO_FEW_ARG_CODE 21
#define ERROR_TOO_FEW_ARG_STR_CODE "21"
#define ERROR_TOO_FEW_ARG_MSG "too few arguments"
#define ERROR_FAULT_ARG_TYP_CODE 22
#define ERROR_FAULT_ARG_TYP_STR_CODE "22"
#define ERROR_FAULT_ARG_TYP_MSG "fault arguments type"
#define ERROR_ARG_OUT_OF_BND_CODE 31
#define ERROR_ARG_OUT_OF_BND_STR_CODE "31"
#define ERROR_ARG_OUT_OF_BND_MSG "argument out of bound"
#define ERROR_LINE_OVFL_CODE 41
#define ERROR_LINE_OVFL_STR_CODE "41"
#define ERROR_LINE_OVFL_MSG "line overflow"


/* exported macro ------------------------------------------------------------*/


/* exported variables --------------------------------------------------------*/
extern QueueHandle_t gp_rx_message;


/* exported function prototypes ----------------------------------------------*/
extern void taskControllerInit();


#endif /* TASKCONTROLLER_H_ */

/**
 * @}
 */
