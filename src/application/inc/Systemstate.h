/**
 * \file        Systemstate.h
 * \brief       global structure for system modules control
 * \date        2014-03-31
 * \version     0.0.1
 *
 * \addtogroup  systemstate
 * @{
 */

#ifndef SYSTEMSTATE_H_
#define SYSTEMSTATE_H_

/* exported typedef ----------------------------------------------------------*/
/**
 * \brief data-type for the global system state
 */
typedef struct
{
	char message[16];		/*!< rx message*/
	uint8_t speed;			/*!< motor speed from 0...100%*/
	uint8_t flash_time;	/*!< */
	uint8_t comm_echo;
	uint8_t comm_respmsg;
}systemstate_t;

/* exported define -----------------------------------------------------------*/
#define ON 1
#define OFF 0

/* exported macro ------------------------------------------------------------*/
/* exported variables --------------------------------------------------------*/
extern systemstate_t g_systemstate;

/* exported function prototypes ----------------------------------------------*/


#endif /* SYSTEMSTATE_H_ */

/**
 * @}
 */
