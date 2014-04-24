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
	.message = "",
	.speed = 100,
	.flash_time = 15,
	.comm_echo = 1,
	.comm_respmsg = 1
};
/* private function prototypes -----------------------------------------------*/
/* private functions ---------------------------------------------------------*/
/* public functions ----------------------------------------------------------*/

/**
 * @}
 */
