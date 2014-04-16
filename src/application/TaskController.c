/**
 * \file        TaskController.c
 * \brief       processing the received messages from TaskCommunicationRX module and set the system state
 * \date        2014-03-31
 * \version     0.0.1
 *
 * \addtogroup  controller
 * \brief       controls the system flow
 * @{
 */
/* Includes ------------------------------------------------------------------*/
/* standard libraries */
#include <stdint.h>

/* RTOS */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

/* BSP */
#include "bsp_led.h"
#include "bsp_gp22.h"

/* application */
#include "inc/Systemstate.h"
#include "inc/TaskCommunicationTX.h" /*!< dependency for message queue */
#include "inc/TaskController.h"

/* private typedef -----------------------------------------------------------*/

//typedef (*msg_filter_t)(char*)(*msg_filter_t)(char*);

typedef union
{
	char string[16];
	uint8_t speed;
	uint8_t error;

} command_param_t;

typedef void (*command_exe_t)(command_param_t*);

typedef struct
{
	command_param_t param;
	command_exe_t func;
}command_t;

typedef uint8_t (*msg_filter_t)(char*, void*, command_t*);

/* private define ------------------------------------------------------------*/
/* message filter patterns */
/* first level */
#define CMD_SET_INITIAL 's'
#define CMD_SET "set"
/* second level */
#define CMD_COMM_INITIAL 'c'
#define CMD_COMM "comm"
#define CMD_COMM_ECHO_INITIAL 'e'
#define CMD_COMM_ECHO "echo"
#define CMD_COMM_RESPMSG_INITIAL 'r'
#define CMD_COMM_RESPMSG "respmsg"
#define CMD_ENGINE_INITIAL 'e'
#define CMD_ENGINE "engine"
#define CMD_ENGINE_SPD_INITIAL 's'
#define CMD_ENGINE_SPD "speed"
#define CMD_STROBO_INITIAL 's'
#define CMD_STROBO "strobo"
#define CMD_STROBO_BRIGHT_INITIAL 'b'
#define CMD_STROBO_BIRGHT "brightness"


/* private macro -------------------------------------------------------------*/
/* private variables ---------------------------------------------------------*/
QueueHandle_t gp_rx_message;

/* private function prototypes -----------------------------------------------*/
uint8_t firstLevelFilter(char*, void*, command_t*);
uint8_t secondLevelSetFilter(char*, void*, command_t*);
void handlerError(command_param_t* param);


/* private functions ---------------------------------------------------------*/
/**
 *	\fn		taskController
 *	\brief	controller task
 *
 *	\param[in]	pvParameters	necessary data for the controller task
 */
static void taskController(void* pvParameters)
{
	/* local variables */
	char rx_message[MESSAGES_MAX_LENGTH];
	command_t command_handler;
	msg_filter_t filter_handler = firstLevelFilter;
	uint8_t next_level;

	/* endless loop */
	for(;;)
	{
		/* wait until a message is received */
		if(xQueueReceive(gp_rx_message,rx_message,portMAX_DELAY) == pdTRUE)
		{
			do
			{
				next_level = (msg_filter_t)filter_handler(rx_message, filter_handler, &command_handler);
			}
			while(next_level);

			command_handler.func(&command_handler.param);
			filter_handler = firstLevelFilter;
		}

	}
}


/* public functions ----------------------------------------------------------*/

/**
 * \fn		taskControllerInit
 * \brief	creates the controller task
 */
void taskControllerInit()
{
	/* create task */
	xTaskCreate(taskController, CONTROLLER_TASK_NAME,
			CONTROLLER_TASK_STACK_SIZE, NULL, CONTROLLER_TASK_PRIORITY, NULL );

	/* create queue with MESSAGES_MAX_LENGTH char space per item */
	gp_rx_message = xQueueCreate(CONTROLLER_QUEUE_LENGHT, sizeof(char[MESSAGES_MAX_LENGTH]));

	/* init modules */
	bsp_LedInit();
	bsp_GP22Init();

	bsp_GP22SendOpcode(0xFF);
}

uint8_t firstLevelFilter(char* msg, void* level_func, command_t* command)
{
	/* local variable */
	uint8_t i;
	uint8_t next_level = 1;
	char set_msg[] = CMD_SET;

	switch(*msg)
	{
	case CMD_SET_INITIAL:
		/* check "set" string */
		for(i=0; i<sizeof(set_msg); i++)
		{
			if(set_msg[i] != *(msg+i))
			{
				next_level = 0;
				break;
			}
		}
		if(next_level)
		{
			level_func = secondLevelSetFilter;
		}
		break;

	/* unknown command */
	//default:

	}

	if(!next_level)
	{
		command->param.error = ERROR_UNKNOW_CMD_CODE;
		command->func = handlerError;
	}

	return next_level;
}


uint8_t secondLevelSetFilter(char* msg, void* level_func, command_t* command)
{
	/* local variable */
	uint8_t i;
	uint8_t next_level = 1;
	char set_msg[] = CMD_SET;

	switch(*msg)
	{
	case CMD_SET_INITIAL:
		/* check "set" string */
		for(i=0; i<sizeof(set_msg); i++)
		{
			if(set_msg[i] != *(msg+i))
			{
				next_level = 0;
			}
		}
		if(next_level)
		{

		}
		break;

	/* unknown command */
	//default:

	}

	return next_level;
}


void handlerError(command_param_t* param)
{

}

/**
 * @}
 */

