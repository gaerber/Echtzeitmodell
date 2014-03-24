/**
 * \file        bsp_serial.h
 * \brief       Board support package for the user communication over UART.
 * \date        2014-03-18
 * \version     0.1
 * \author		Kevin Gerber
 *
 * \addtogroup  bsp
 * @{
 *
 * \addtogroup  bsp_serial
 * \brief		The BSP_SERIAL module provides a function to initialize the UART
 * 				port for the user communication. The UART will be configured in
 * 				interrupt mode. In this mode there are two user functions to handle
 * 				the RX and RX interrupt. They must be implemented by the operator.
 * 				They are called \var bsp_SerialIrqTxHandler() and \var bsp_SerialIrqRxHandler()
 *
 * @{
 */

#ifndef BSP_SERIAL_H_
#define BSP_SERIAL_H_

/** Hardware label from the UART RX pin */
static const bsp_gpioconf_t BSP_SERIAL_RX = {
		RCC_AHB1Periph_GPIOA, GPIOA, GPIO_Pin_10, GPIO_AF_USART1
};

/** Hardware label from the UART TX pin */
static const bsp_gpioconf_t BSP_SERIAL_TX = {
		RCC_AHB1Periph_GPIOA, GPIOA, GPIO_Pin_9, GPIO_AF_USART1
};

#define BSP_SERIAL_PORT			USART1					/*!< Port base address of the UART port */
#define BSP_SERIAL_PERIPH		RCC_APB2Periph_USART1	/*!< RCC AHB peripheral of the UART port */

#define BSP_SERIAL_IRQ_CHANEL	USART1_IRQn			/*!< NVIC UART interrupt */
#define BSP_SERIAL_IRQ_PRIORITY	0					/*!< NVIC UART interrupt priority */
#define BSP_SERIAL_IRQ_Handler	USART1_IRQHandler	/*!< NVIC interrupt handler */

/* UART settings */
#define BSP_SERIAL_UART_BAUD	115200				/*!< UART baud */
#define BSP_SERIAL_UART_LENGTH	USART_WordLength_8b	/*!< UART word lngth */
#define BSP_SERIAL_UART_STOP	USART_StopBits_1	/*!< UART number of stop bits */
#define BSP_SERIAL_UART_PARITY	USART_Parity_No		/*!< UART parity bit */
#define BSP_SERIAL_UART_FLOW	USART_HardwareFlowControl_None	/*!< UART hardware flow control */

/* Function prototypes */
extern void bsp_SerialInit(void);
extern void bsp_SerialSend(uint16_t data);
extern void bsp_SerialReceive(uint16_t *data);
extern void bspSerialTxIrqEnable(void);
extern void bspSerialTxIrqDisable(void);

#endif /* BSP_SERIAL_H_ */

/**
 * @}
 */

/**
 * @}
 */
