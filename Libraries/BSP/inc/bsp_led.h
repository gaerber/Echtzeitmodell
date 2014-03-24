/**
 * \file        bsp_led.h
 * \brief       Board support package to use all the LEDs.
 * \date        2014-03-18
 * \version     0.1
 * \author		Kevin Gerber
 *
 * \addtogroup  bsp
 * @{
 *
 * \addtogroup  bsp_led
 * \brief		????
 * @{
 */

#ifndef BSP_LED_H_
#define BSP_LED_H_

/** LED is in ON state. */
#define BSP_LED_ON			1
/** LED is in OFF state. */
#define BSP_LED_OFF			0

/**
 * \brief		List off all LEDs to use in software.
 * \important	All LEDs require an entry in the \var port_led array with the correct hardware labels.
 */
typedef enum {
	BSP_LED_GREEN = 0,		/*!< The green LED identification number. */
	BSP_LED_RED,			/*!< The red LED identification number. */
	BSP_LED_ELEMENTCTR		/*!< Counts the LEDs. It must be the last element. Do not use as LED! */
} bsp_led_t;

/**
 * \brief	Array off all used LED with their correct hardware label.
 */
static const bsp_gpioconf_t port_led[] = {
	{RCC_AHB1Periph_GPIOI, GPIOI, GPIO_Pin_6},	/* BSP_LED_GREEN */
	{RCC_AHB1Periph_GPIOA, GPIOA, GPIO_Pin_3}	/* BSP_LED_RED */
};

/* Function prototypes */
extern void bsp_LedInit(void);
extern void bsp_LedSetOn(bsp_led_t led);
extern void bsp_LedSetOff(bsp_led_t led);
extern void bsp_LedSetToggle(bsp_led_t led);
//extern uint8_t bsp_LedGet(bsp_led_t led);

#endif /* BSP_LED_H_ */

/**
 * @}
 */

/**
 * @}
 */
