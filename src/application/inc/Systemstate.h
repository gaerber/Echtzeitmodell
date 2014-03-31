/**
 * \file        Systemstate.h
 * \brief       gobal structure for system modules control
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
	uint16_t flash_time;	/*!< */
	enum
	{
		STATE_CONFIG = 0,	/*!< system is in the config-state*/
		STATE_RUN			/*!< system runs -> text is visible*/
	}system_state;
}systemstate_t;

/* exported define -----------------------------------------------------------*/
/* exported macro ------------------------------------------------------------*/
/* exported variables --------------------------------------------------------*/
extern systemstate_t g_systemstate;

/* exported function prototypes ----------------------------------------------*/


#endif /* SYSTEMSTATE_H_ */

/**
 * @}
 */
