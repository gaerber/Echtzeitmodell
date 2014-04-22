/**
 * \file        TaskController.c
 * \brief       processing the received messages from TaskCommunicationRX module and set the system state
 * \date        2014-03-31
 * \version     0.0.1
 *
 * \todo		POINTER PROBLEME
 *
 * \addtogroup  controller
 * \brief       controls the system flow
 * @{
 */
/* Includes ------------------------------------------------------------------*/
/* standard libraries */
#include <stdint.h>
#include <string.h>

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

typedef union
{
	char new[16];
	uint8_t echo; /*!< 1 := ON; 0 := OFF */
	uint8_t respmsg;
	uint8_t speed;
	uint8_t brightness;
	uint8_t error;
} command_param_t;

typedef void (*command_exe_t)(command_param_t*);

typedef struct
{
	command_param_t param;
	command_exe_t func;
}command_t;

typedef uint8_t (*msg_filter_t)(char**, void**, command_t*);

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
#define CMD_STROBO_BRIGHT "brightness"


/* private macro -------------------------------------------------------------*/
/* private variables ---------------------------------------------------------*/
QueueHandle_t gp_rx_message;

/* private function prototypes -----------------------------------------------*/
uint8_t firstLevelFilter(char**, void**, command_t*);
uint8_t secondLevelSetFilter(char**, void**, command_t*);
uint8_t thirdLevelSetCommFilter(char**, void**, command_t*);
uint8_t thirdLevelSetEngineFilter(char**, void**, command_t*);
uint8_t thirdLevelSetStroboFilter(char**, void**, command_t*);
uint8_t fourdLevelSetCommEchoFilter(char**, void**, command_t*);
uint8_t fourdLevelSetCommRespmsgFilter(char**, void**, command_t*);
uint8_t fourdLevelSetEngineSpdFilter(char**, void**, command_t*);
uint8_t fourdLevelSetStroboBrightFilter(char**, void**, command_t*);

void handlerError(command_param_t*);
void handlerEcho(command_param_t*);
void handlerRespmsg(command_param_t*);
void handlerSpd(command_param_t*);
void handlerBright(command_param_t*);


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
	char rx_message[MESSAGES_MAX_LENGTH];// ="set comm echo on";
	char* start_ptr;
	command_t command_handler;
	msg_filter_t filter_handler;
	uint8_t next_level;

	/* endless loop */
	for(;;)
	{
		filter_handler = firstLevelFilter;

		/* wait until a message is received */
		if(xQueueReceive(gp_rx_message,rx_message,portMAX_DELAY) == pdTRUE)
		{
			start_ptr = &rx_message[0]; /* prevent null-pointer */
			do
			{
				/* find first character */
				while(*start_ptr)
				{
					if(*start_ptr != ' ')
					{
						break;
					}
					start_ptr++;
				}

				next_level = filter_handler(&start_ptr,(void**)(&filter_handler), &command_handler);
			}
			while(next_level);


			if(*rx_message != '\0')
			{
				command_handler.func(&command_handler.param);
			}

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


/**
 * \fn 		checkCommand
 * \brief	string compare
 *
 * \param	msg received message
 * \param	cmd	command string
 * \param	len	lenght of command string
 *
 * \retval  0 strings doesn't match
 * \retval	1 strings are equal
 */
uint8_t checkCommand(char*** msg, const char* cmd, uint8_t len)
{
	uint8_t i;
	uint8_t next_level = 1;
	char msg_tmp;
	char cmd_tmp;


	for(i=0; i<len-1; i++)
	{
		cmd_tmp = *(cmd+i);
		msg_tmp = *(*(*(msg))+i);

		if(cmd_tmp != msg_tmp)
		{
			next_level = 0;
		}
	}

	*(*msg) = *(*(msg))+i;

	return next_level;
}


/**
 * \fn		firstLevelFilter
 * \brief	first filter level
 *
 * \param	msg	pointer to received message
 * \param	level_func pointer to next filter level
 * \param	command pointer to execute function
 */
uint8_t firstLevelFilter(char** msg, void** level_func, command_t* command)
{
	/* local variable */
	uint8_t next_level;
	const char set_msg[] = CMD_SET;

	switch(*(*msg))
	{
	case CMD_SET_INITIAL:
		/* check "set" string */
		next_level = checkCommand(&msg, set_msg, sizeof(set_msg));

		if(next_level)
		{
			*level_func = (void*)secondLevelSetFilter;
		}
		break;

	/* unknown command */
	default:
		next_level = 0;
	}

	/* set error */
	if(!next_level)
	{
		command->param.error = ERROR_UNKNOW_CMD_CODE;
		command->func = handlerError;
	}

	return next_level;
}


/**
 * \fn		secondLevelSetFilter
 * \brief	second filter level
 *
 * \param	msg	pointer to received message
 * \param	level_func pointer to next filter level
 * \param	command pointer to execute function
 */
uint8_t secondLevelSetFilter(char** msg, void** level_func, command_t* command)
{
	/* local variable */
	uint8_t next_level;
	const char comm_msg[] = CMD_COMM;
	const char engine_msg[] = CMD_ENGINE;
	const char strobo_msg[] = CMD_STROBO;

	switch(*(*msg))
	{
	case CMD_COMM_INITIAL:
		/* check "comm" string */
		next_level = checkCommand(&msg, comm_msg, sizeof(comm_msg));

		if(next_level)
		{
			*level_func = (void*)thirdLevelSetCommFilter;
		}
		break;

	case CMD_ENGINE_INITIAL:
		/* check "engine" string */
		next_level = checkCommand(&msg, engine_msg, sizeof(engine_msg));

		if(next_level)
		{
			*level_func = (void*)thirdLevelSetEngineFilter;
		}

		break;

	case CMD_STROBO_INITIAL:
		/* check "strobo" string */
		next_level = checkCommand(&msg, strobo_msg, sizeof(strobo_msg));

		if(next_level)
		{
			*level_func = (void*)thirdLevelSetStroboFilter;
		}

		break;

	/* unknown command */
	default:
		next_level = 0;
	}

	/* set error */
	if(!next_level)
	{
		command->param.error = ERROR_UNKNOW_CMD_CODE;
		command->func = handlerError;
	}

	return next_level;
}





/**
 * \fn		thirdLevelSetCommFilter
 */
uint8_t thirdLevelSetCommFilter(char** msg, void** level_func, command_t* command)
{
	/* local variable */
	uint8_t next_level;
	const char comm_echo_msg[] = CMD_COMM_ECHO;
	const char comm_respmsg_msg[] = CMD_COMM_RESPMSG;

	switch(*(*msg))
	{
	case CMD_COMM_ECHO_INITIAL:
		/* check "echo" string */
		next_level = checkCommand(&msg, comm_echo_msg, sizeof(comm_echo_msg));

		if(next_level)
		{
			*level_func = (void*)fourdLevelSetCommEchoFilter;
		}
		break;

	case CMD_COMM_RESPMSG_INITIAL:
		/* check "response" string */
		next_level = checkCommand(&msg, comm_respmsg_msg, sizeof(comm_respmsg_msg));

		if(next_level)
		{
			*level_func = (void*)fourdLevelSetCommRespmsgFilter;
		}

		break;

	/* unknown command */
	default:
		next_level = 0;
	}

	/* set error */
	if(!next_level)
	{
		command->param.error = ERROR_UNKNOW_CMD_CODE;
		command->func = handlerError;
	}

	return next_level;
}


/**
 * \fn		thirdLevelSetEngineFilter
 */
uint8_t thirdLevelSetEngineFilter(char** msg, void** level_func, command_t* command)
{
	/* local variable */
	uint8_t next_level;
	const char engine_spd_msg[] = CMD_ENGINE_SPD;

	switch(*(*msg))
	{
	case CMD_ENGINE_SPD_INITIAL:
		/* check "speed" string */
		next_level = checkCommand(&msg, engine_spd_msg, sizeof(engine_spd_msg));

		if(next_level)
		{
			*level_func = (void*)fourdLevelSetEngineSpdFilter;
		}
		break;

	/* unknown command */
	default:
		next_level = 0;
	}

	/* set error */
	if(!next_level)
	{
		command->param.error = ERROR_UNKNOW_CMD_CODE;
		command->func = handlerError;
	}

	return next_level;
}


/**
 * \fn		thirdLevelSetStroboFilter
 */
uint8_t thirdLevelSetStroboFilter(char** msg, void** level_func, command_t* command)
{
	/* local variable */
	uint8_t next_level;
	const char strobo_bright_msg[] = CMD_STROBO_BRIGHT;

	switch(*(*msg))
	{
	case CMD_STROBO_BRIGHT_INITIAL:
		/* check "brightness" string */
		next_level = checkCommand(&msg, strobo_bright_msg, sizeof(strobo_bright_msg));

		if(next_level)
		{
			*level_func = (void*)fourdLevelSetStroboBrightFilter;
		}
		break;

	/* unknown command */
	default:
		next_level = 0;
	}

	/* set error */
	if(!next_level)
	{
		command->param.error = ERROR_UNKNOW_CMD_CODE;
		command->func = handlerError;
	}

	return next_level;
}






/**
 * \fn		fourdLevelSetCommEchoFilter
 * \todo    read value
 */
uint8_t fourdLevelSetCommEchoFilter(char** msg, void** level_func, command_t* command)
{
	/* local variables */

	if(strcmp(*msg,"on") == 0)
	{
		command->func = handlerEcho;
		command->param.echo = 1;
	}
	else if(strcmp(*msg,"off") == 0)
	{
		command->func = handlerEcho;
		command->param.echo = 0;
	}
	else
	{
		command->param.error = ERROR_FAULT_ARG_TYP_CODE;
		command->func = handlerError;
	}

	return 0;
}


/**
 * \fn		fourdLevelSetCommRespmsgFilter
 * \todo    read value
 */
uint8_t fourdLevelSetCommRespmsgFilter(char** msg, void** level_func, command_t* command)
{
	ERROR_FAULT_ARG_TYP_CODE;
	ERROR_TOO_FEW_ARG_CODE;
	ERROR_ARG_OUT_OF_BND_CODE;

	/* set error */
//	if(!next_level)
//	{
//		command->param.error = ERROR_UNKNOW_CMD_CODE;
//		command->func = handlerError;
//	}
//
//	return next_level;
}


/**
 * \fn		fourdLevelSetEngineSpdFilter
 * \todo    read value
 */
uint8_t fourdLevelSetEngineSpdFilter(char** msg, void** level_func, command_t* command)
{
	ERROR_FAULT_ARG_TYP_CODE;
	ERROR_TOO_FEW_ARG_CODE;
	ERROR_ARG_OUT_OF_BND_CODE;

//	/* set error */
//	if(!next_level)
//	{
//		command->param.error = ERROR_UNKNOW_CMD_CODE;
//		command->func = handlerError;
//	}
//
//	return next_level;
}


/**
 * \fn		fourdLevelSetStroboBrightFilter
 * \todo    read value
 */
uint8_t fourdLevelSetStroboBrightFilter(char** msg, void** level_func, command_t* command)
{
	ERROR_FAULT_ARG_TYP_CODE;
	ERROR_TOO_FEW_ARG_CODE;
	ERROR_ARG_OUT_OF_BND_CODE;

//	/* set error */
//	if(!next_level)
//	{
//		command->param.error = ERROR_UNKNOW_CMD_CODE;
//		command->func = handlerError;
//	}
//
//	return next_level;
}


void handlerError(command_param_t* param)
{

}


void handlerEcho(command_param_t* param)
{
	char str[] = ERROR_ACK_STR_CODE" " ERROR_ACK_MSG"\r\n";

	g_systemstate.comm_echo = param->echo;

	if(g_systemstate.comm_respmsg)
	{
		xQueueSend(gq_tx_message,str,portMAX_DELAY);
	}
	else
	{
		xQueueSend(gq_tx_message,ERROR_ACK_MSG,portMAX_DELAY);
	}
}

/**
 * @}
 */
