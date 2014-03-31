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
/* standard librarys */
#include <stdint.h>

/* application */
#include "inc/Systemstate.h"

/* private typedef -----------------------------------------------------------*/
/* private define ------------------------------------------------------------*/
/* private macro -------------------------------------------------------------*/
/* private variables ---------------------------------------------------------*/
systemstate_t g_systemstate =
{
	.message = {0},
	.speed = 0,
	.flash_time = 0,
	.system_state = STATE_CONFIG
};
/* private function prototypes -----------------------------------------------*/
/* private functions ---------------------------------------------------------*/
/* public functions ----------------------------------------------------------*/

/**
 * @}
 */
