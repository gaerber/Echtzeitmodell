/**
 * \file        TaskMeasurement.h
 * \brief       calculate the light trigger
 * \date        2014-03-31
 * \version     0.0.1
 *
 * \addtogroup  measurement
 * @{
 */

#ifndef TASKMEASUREMENT_H_
#define TASKMEASUREMENT_H_

/* exported typedef ----------------------------------------------------------*/
/* exported define -----------------------------------------------------------*/
#define MEASUREMENT_TASK_PRIORITY	2
#define MEASUREMENT_TASK_NAME		"Task Measurement"
#define MEASUREMENT_TASK_STACK_SIZE	configMINIMAL_STACK_SIZE

/* exported macro ------------------------------------------------------------*/
/* exported variables --------------------------------------------------------*/
/* exported function prototypes ----------------------------------------------*/
extern void taskMeasurementInit();


#endif /* TASKMEASUREMENT_H_ */

/**
 * @}
 */
