/**
 * \file        Systemstate.c
 * \brief       gobal structure for system modules control
 * \date        2014-03-31
 * \version     0.0.1
 *
 * \addtogroup  systemstate
 * \brief       global state structure
 * @{
 */
/* Includes ------------------------------------------------------------------*/
/* standard libraries */
#include <stdint.h>

/* application */
#include "inc/Systemstate.h"

/* private typedef -----------------------------------------------------------*/
/* private define ------------------------------------------------------------*/
/* private macro -------------------------------------------------------------*/
/* private variables ---------------------------------------------------------*/
systemstate_t g_systemstate =
{
	.message = "kevin !",
	.speed = 100,
	.flash_time = 0,
	.comm_echo = 1,
	.system_state = STATE_CONFIG
};
/* private function prototypes -----------------------------------------------*/
/* private functions ---------------------------------------------------------*/
/* public functions ----------------------------------------------------------*/

/**
 * @}
 */
