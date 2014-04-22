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
#include <stdlib.h>


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


/**
 * \brief extracted parameters from the string-message
 */
typedef union
{
	char new[16];
	uint8_t echo; /*!< 1 := ON; 0 := OFF */
	uint8_t respmsg; /*!< 1:= message + message code; 0:= only the message */
	uint8_t speed; /*!< speed in % from 0 to 100 */
	uint8_t brightness; /*!< brightness of the flashlight from 0 to 100 */
	uint8_t error; /*!< error ID's, \see TaskController.h */
} command_param_t;


/**
 * \brief function for the filter result
 */
typedef void (*command_exe_t)(command_param_t*);




/**
 * \brief summary of the command-function and parameters
 */
typedef struct
{
	command_param_t param;
	command_exe_t func;
}command_t;




/*
 * \brief call function for the different filter-levels
 */
typedef uint8_t (*msg_filter_t)(char**, void**, command_t*);


/* private define ------------------------------------------------------------*/
/* message filter patterns */
/* first level */
#define CMD_SET_INITIAL 's'
#define CMD_SET "set"
/* second level */
#define CMD_COMM_INITIAL 'c'
#define CMD_COMM "comm"
#define CMD_ENGINE_INITIAL 'e'
#define CMD_ENGINE "engine"
#define CMD_STROBO_INITIAL 's'
#define CMD_STROBO "strobo"
#define CMD_MESSAGE_INITIAL 'm'
#define CMD_MESSAGE "message"
/* third level */
#define CMD_COMM_ECHO_INITIAL 'e'
#define CMD_COMM_ECHO "echo"
#define CMD_COMM_RESPMSG_INITIAL 'r'
#define CMD_COMM_RESPMSG "respmsg"
#define CMD_ENGINE_SPD_INITIAL 's'
#define CMD_ENGINE_SPD "speed"
#define CMD_STROBO_BRIGHT_INITIAL 'b'
#define CMD_STROBO_BRIGHT "brightness"
#define CMD_MESSAGE_NEW_INITIAL 'n'
#define CMD_MESSAGE_NEW "new"




/* private macro -------------------------------------------------------------*/
/* private variables ---------------------------------------------------------*/
QueueHandle_t gp_rx_message;


/* private function prototypes -----------------------------------------------*/
/* filter levels */
static uint8_t firstLevelFilter(char**, void**, command_t*);
static uint8_t secondLevelSetFilter(char**, void**, command_t*);
static uint8_t thirdLevelSetCommFilter(char**, void**, command_t*);
static uint8_t thirdLevelSetEngineFilter(char**, void**, command_t*);
static uint8_t thirdLevelSetStroboFilter(char**, void**, command_t*);
static uint8_t thirdLevelSetMessageFilter(char**, void**, command_t*);
static uint8_t fourthLevelSetCommEchoFilter(char**, void**, command_t*);
static uint8_t fourthLevelSetCommRespmsgFilter(char**, void**, command_t*);
static uint8_t fourthLevelSetEngineSpdFilter(char**, void**, command_t*);
static uint8_t fourthLevelSetStroboBrightFilter(char**, void**, command_t*);
static uint8_t fourthLevelSetMessageNewFilter(char**, void**, command_t*);


/* handlers */
static void handlerError(command_param_t*);
static void handlerEcho(command_param_t*);
static void handlerRespmsg(command_param_t*);
static void handlerSpd(command_param_t*);
static void handlerBright(command_param_t*);
static void handlerMessage(command_param_t*);


/* plot ACK message */
static void printACK(void);




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
		/* set first filter stage */
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


				/* filtering of the current filter stage */
				next_level = filter_handler(&start_ptr,(void**)(&filter_handler), &command_handler);
			}
			while(next_level);




//			if(*rx_message != '\0')
//			{
				/* call filter-result handler */
				command_handler.func(&command_handler.param);
//			}


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
}




/**
 * \fn 		checkCommand
 * \brief	string compare like strcmp, but without '\0'
 *
 * \param	msg received message
 * \param	cmd	command string
 * \param	len	lenght of command string
 *
 * \retval  0 strings doesn't match
 * \retval	1 strings are equal
 */
static uint8_t checkCommand(char*** msg, const char* cmd, uint8_t len)
{
	uint8_t i;
	uint8_t next_level = 1;
	char msg_tmp;
	char cmd_tmp;


	/* compare every letter */
	for(i=0; i<len-1; i++)
	{
		cmd_tmp = *(cmd+i);
		msg_tmp = *(*(*(msg))+i);


		if(cmd_tmp != msg_tmp)
		{
			next_level = 0;
		}
	}


	/* adjust message pointer for pointing to next space ' ' */
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
static uint8_t firstLevelFilter(char** msg, void** level_func, command_t* command)
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
static uint8_t secondLevelSetFilter(char** msg, void** level_func, command_t* command)
{
	/* local variable */
	uint8_t next_level;
	const char comm_msg[] = CMD_COMM;
	const char engine_msg[] = CMD_ENGINE;
	const char strobo_msg[] = CMD_STROBO;
	const char message_msg[] = CMD_MESSAGE;


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


	case CMD_MESSAGE_INITIAL:
		/* check "message" string */
		next_level = checkCommand(&msg, message_msg, sizeof(message_msg));


		if(next_level)
		{
			*level_func = (void*)thirdLevelSetMessageFilter;
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
 * \brief	third filter level for "comm"
 *
 * \param	msg	pointer to received message
 * \param	level_func pointer to next filter level
 * \param	command pointer to execute function
 */
static uint8_t thirdLevelSetCommFilter(char** msg, void** level_func, command_t* command)
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
			*level_func = (void*)fourthLevelSetCommEchoFilter;
		}
		break;


	case CMD_COMM_RESPMSG_INITIAL:
		/* check "response" string */
		next_level = checkCommand(&msg, comm_respmsg_msg, sizeof(comm_respmsg_msg));


		if(next_level)
		{
			*level_func = (void*)fourthLevelSetCommRespmsgFilter;
		}


		break;


	/* unknown command */
	default:
		next_level = 0;
	}


	/* set error */
	if(!next_level)
	{
		/* no parameter aviable */
		if(**msg == '\0')
		{
			command->param.error = ERROR_TOO_FEW_ARG_CODE;
			command->func = handlerError;
		}
		else
		{
			command->param.error = ERROR_UNKNOW_CMD_CODE;
			command->func = handlerError;
		}
	}


	return next_level;
}




/**
 * \fn		thirdLevelSetEngineFilter
 * \brief	third filter level for "engine"
 *
 * \param	msg	pointer to received message
 * \param	level_func pointer to next filter level
 * \param	command pointer to execute function
 */
static uint8_t thirdLevelSetEngineFilter(char** msg, void** level_func, command_t* command)
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
			*level_func = (void*)fourthLevelSetEngineSpdFilter;
		}
		break;


	/* unknown command */
	default:
		next_level = 0;
	}


	/* set error */
	if(!next_level)
	{
		/* no parameter aviable */
		if(**msg == '\0')
		{
			command->param.error = ERROR_TOO_FEW_ARG_CODE;
			command->func = handlerError;
		}
		else
		{
			command->param.error = ERROR_UNKNOW_CMD_CODE;
			command->func = handlerError;
		}
	}


	return next_level;
}




/**
 * \fn		thirdLevelSetStroboFilter
 * \brief	third filter level for "strobo"
 *
 * \param	msg	pointer to received message
 * \param	level_func pointer to next filter level
 * \param	command pointer to execute function
 */
static uint8_t thirdLevelSetStroboFilter(char** msg, void** level_func, command_t* command)
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
			*level_func = (void*)fourthLevelSetStroboBrightFilter;
		}
		break;


	/* unknown command */
	default:
		next_level = 0;
	}


	/* set error */
	if(!next_level)
	{
		/* no parameter aviable */
		if(**msg == '\0')
		{
			command->param.error = ERROR_TOO_FEW_ARG_CODE;
			command->func = handlerError;
		}
		else
		{
			command->param.error = ERROR_UNKNOW_CMD_CODE;
			command->func = handlerError;
		}
	}


	return next_level;
}




/**
 * \fn		thirdLevelSetMessageFilter
 * \brief	third filter level for "message"
 *
 * \param	msg	pointer to received message
 * \param	level_func pointer to next filter level
 * \param	command pointer to execute function
 */
static uint8_t thirdLevelSetMessageFilter(char** msg, void** level_func, command_t* command)
{
	/* local variable */
	uint8_t next_level;
	const char message_new_msg[] = CMD_MESSAGE_NEW;


	switch(*(*msg))
	{
	case CMD_MESSAGE_NEW_INITIAL:
		/* check "new" string */
		next_level = checkCommand(&msg, message_new_msg, sizeof(message_new_msg));


		if(next_level)
		{
			*level_func = (void*)fourthLevelSetMessageNewFilter;
		}
		break;


	/* unknown command */
	default:
		next_level = 0;
	}


	/* set error */
	if(!next_level)
	{
		/* no parameter aviable */
		if(**msg == '\0')
		{
			command->param.error = ERROR_TOO_FEW_ARG_CODE;
			command->func = handlerError;
		}
		else
		{
			command->param.error = ERROR_UNKNOW_CMD_CODE;
			command->func = handlerError;
		}
	}


	return next_level;
}


/**
 * \fn		fourthLevelSetCommEchoFilter
 * \brief	fourth filter level for "echo"
 *
 * \param	msg	pointer to received message
 * \param	level_func pointer to next filter level
 * \param	command pointer to execute function
 */
static uint8_t fourthLevelSetCommEchoFilter(char** msg, void** level_func, command_t* command)
{
	/* check if the parameter is a string */
	if(**msg >= 'a' && **msg <= 'z')
	{
		/* check if the parameter is "on" or "off" */
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
		/* set error ID */
		else
		{
			command->param.error = ERROR_ARG_OUT_OF_BND_CODE;
			command->func = handlerError;
		}
	}
	/* set error ID */
	else
	{
		command->param.error = ERROR_FAULT_ARG_TYP_CODE;
		command->func = handlerError;
	}


	/* end of filtering */
	return 0;
}




/**
 * \fn		fourthLevelSetCommRespmsgFilter
 * \brief	fourth filter level for "respmsg"
 *
 * \param	msg	pointer to received message
 * \param	level_func pointer to next filter level
 * \param	command pointer to execute function
 */
static uint8_t fourthLevelSetCommRespmsgFilter(char** msg, void** level_func, command_t* command)
{
	/* check if the parameter is a string */
	if(**msg >= 'a' && **msg <= 'z')
	{
		/* check if the parameter is "on" or "off" */
		if(strcmp(*msg,"on") == 0)
		{
			command->func = handlerRespmsg;
			command->param.respmsg = 1;
		}
		else if(strcmp(*msg,"off") == 0)
		{
			command->func = handlerRespmsg;
			command->param.respmsg = 0;
		}
		/* set error ID */
		else
		{
			command->param.error = ERROR_ARG_OUT_OF_BND_CODE;
			command->func = handlerError;
		}
	}
	/* set error ID */
	else
	{
		command->param.error = ERROR_FAULT_ARG_TYP_CODE;
		command->func = handlerError;
	}


	/* end of filtering */
	return 0;
}




/**
 * \fn		fourthLevelSetEngineSpdFilter
 * \brief	fourth filter level for "spd"
 *
 * \param	msg	pointer to received message
 * \param	level_func pointer to next filter level
 * \param	command pointer to execute function
 */
static uint8_t fourthLevelSetEngineSpdFilter(char** msg, void** level_func, command_t* command)
{
	/* local variables */
	uint16_t speed_value = 0;


	/* check if the parameter is a number */
	if(**msg >= '0' && **msg <= '9')
	{
		/* convert ascii to int */
		speed_value = atoi(*msg);


		/* check if value within the bounds */
		if(speed_value > 0 && speed_value <= 100)
		{
			command->func = handlerSpd;
			command->param.speed = speed_value;
		}
		/* set error ID */
		else
		{
			command->param.error = ERROR_ARG_OUT_OF_BND_CODE;
			command->func = handlerError;
		}
	}
	/* set error ID */
	else
	{
		command->param.error = ERROR_FAULT_ARG_TYP_CODE;
		command->func = handlerError;
	}


	/* end of filtering */
	return 0;
}




/**
 * \fn		fourthLevelSetStroboBrightFilter
 * \brief	fourth filter level for "bright"
 *
 * \param	msg	pointer to received message
 * \param	level_func pointer to next filter level
 * \param	command pointer to execute function
 */
static uint8_t fourthLevelSetStroboBrightFilter(char** msg, void** level_func, command_t* command)
{
	/* local variables */
	uint16_t bright_value = 0;


	/* check if the parameter is a number */
	if(**msg >= '0' && **msg <= '9')
	{
		/* convert ascii to int */
		bright_value = atoi(*msg);


		/* check if value within the bounds */
		if(bright_value > 0 && bright_value <= 100)
		{
			command->func = handlerBright;
			command->param.brightness = bright_value;
		}
		/* set error ID */
		else
		{
			command->param.error = ERROR_ARG_OUT_OF_BND_CODE;
			command->func = handlerError;
		}
	}
	/* set error ID */
	else
	{
		command->param.error = ERROR_FAULT_ARG_TYP_CODE;
		command->func = handlerError;
	}


	/* end of filtering */
	return 0;
}




/**
 * \fn		fourthLevelSetMessageNewFilter
 * \brief	fourth filter level for "new"
 *
 * \param	msg	pointer to received message
 * \param	level_func pointer to next filter level
 * \param	command pointer to execute function
 *
 * \todo	test
 */
static uint8_t fourthLevelSetMessageNewFilter(char** msg, void** level_func, command_t* command)
{
	/* check if the string isn't too long */
	if(strlen(*msg) <= 16)
	{
		command->func = handlerMessage;
		strcpy(command->param.new,*msg);
	}
	/* set error ID */
	else
	{
		command->param.error = ERROR_ARG_OUT_OF_BND_CODE;
		command->func = handlerError;
	}


	/* end of filtering */
	return 0;
}




/**
 * \fn 		handlerError
 * \brief	plot error message
 *
 * \param	param	error ID
 */
static void handlerError(command_param_t* param)
{
	/* local variables */
	const char error_unknow[] = ERROR_UNKNOW_CMD_STR_CODE" " ERROR_UNKNOW_CMD_MSG"\r\n";
	const char error_few[] = ERROR_TOO_FEW_ARG_STR_CODE" " ERROR_TOO_FEW_ARG_MSG"\r\n";
	const char error_fault[] = ERROR_FAULT_ARG_TYP_STR_CODE" " ERROR_FAULT_ARG_TYP_MSG"\r\n";
	const char error_bound[] = ERROR_ARG_OUT_OF_BND_STR_CODE" " ERROR_ARG_OUT_OF_BND_MSG"\r\n";


	switch(param->error)
	{
	case ERROR_UNKNOW_CMD_CODE:
		if(g_systemstate.comm_respmsg){xQueueSend(gq_tx_message,error_unknow,portMAX_DELAY);}
		else {xQueueSend(gq_tx_message,ERROR_UNKNOW_CMD_MSG,portMAX_DELAY);}
		break;


	case ERROR_TOO_FEW_ARG_CODE:
		if(g_systemstate.comm_respmsg){xQueueSend(gq_tx_message,error_few,portMAX_DELAY);}
		else {xQueueSend(gq_tx_message,ERROR_TOO_FEW_ARG_MSG,portMAX_DELAY);}
		break;


	case ERROR_FAULT_ARG_TYP_CODE:
		if(g_systemstate.comm_respmsg){xQueueSend(gq_tx_message,error_fault,portMAX_DELAY);}
		else {xQueueSend(gq_tx_message,ERROR_FAULT_ARG_TYP_MSG,portMAX_DELAY);}
		break;


	case ERROR_ARG_OUT_OF_BND_CODE:
		if(g_systemstate.comm_respmsg){xQueueSend(gq_tx_message,error_bound,portMAX_DELAY);}
		else {xQueueSend(gq_tx_message,ERROR_ARG_OUT_OF_BND_MSG,portMAX_DELAY);}
	}
}


/**
 * \fn		handlerEcho
 * \brief	enable/disable echo
 *
 * \param	param	0 := no echo; 1 := echo on
 */
static void handlerEcho(command_param_t* param)
{
	/* set system state */
	g_systemstate.comm_echo = param->echo;


	printACK();
}




/**
 * \fn		handlerRespmsg
 * \brief	set response possibilities (full or just the message)
 *
 * \param	param	0 := just the message; 1 := full
 */
static void handlerRespmsg(command_param_t* param)
{
	/* set system state */
	g_systemstate.comm_respmsg = param->respmsg;


	printACK();
}


/**
 * \fn		handlerSpd
 * \brief	set speed
 *
 * \param	param	value between 1-100
 */
static void handlerSpd(command_param_t* param)
{
	/* set system state */
	g_systemstate.speed = param->speed;


	printACK();
}


/**
 * \fn		handlerBright
 * \brief	set brightness
 *
 * \param	param 	value between 1-100
 */
static void handlerBright(command_param_t* param)
{
	/* set system state */
	g_systemstate.flash_time = param->brightness;


	printACK();
}




/**
 * \fn		handlerMessage
 * \brief	set the new message
 *
 * \param	param	message string with 16 letters
 *
 * \todo 	test
 */
static void handlerMessage(command_param_t* param)
{
	/* set system state */
	strcpy(g_systemstate.message,param->new);


	printACK();
}




/**
 * \fn		printACK
 * \brief	print ACK to the terminal
 */
static void printACK(void)
{
	/* local variable */
	const char str[] = ERROR_ACK_STR_CODE" " ERROR_ACK_MSG"\r\n";


	/* print response (full or only message without ID */
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
