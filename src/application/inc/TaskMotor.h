/**
 * \file        TaskMotor.c
 * \brief       controls the motor speed
 * \date        2014-03-31
 * \version     0.0.1
 *
 * \addtogroup  motor
 * @{
 */

#ifndef TASKMOTOR_H_
#define TASKMOTOR_H_

/* exported typedef ----------------------------------------------------------*/
/* exported define -----------------------------------------------------------*/
#define MOTOR_TASK_PRIORITY			2
#define MOTOR_TASK_NAME				"Task MotorS"
#define MOTOR_TASK_STACK_SIZE		configMINIMAL_STACK_SIZE

#define MOTOR_SPEED_QUEUE_LENGHT	1

/* exported macro ------------------------------------------------------------*/
/* exported variables --------------------------------------------------------*/
extern QueueHandle_t gq_motor_speed;

/* exported function prototypes ----------------------------------------------*/
extern void taskMotorInit();



#endif /* TASKMOTOR_H_ */

/**
 * @}
 */
