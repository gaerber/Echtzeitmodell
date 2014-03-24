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
 * @{
 */

#include "bsp.h"
#include "bsp_serial.h"

extern void bsp_SerialIrqTxHandler(void);
extern void bsp_SerialIrqRxHandler(void);

/**
  * \brief	This function handles USARTx global interrupt request.
  */
void BSP_SERIAL_IRQ_Handler(void) {
	/* UART receive data register not empty */
	if(USART_GetITStatus(BSP_SERIAL_PORT, USART_IT_RXNE) != RESET) {
		bsp_SerialIrqRxHandler();
	}

	/* UART transmit data register is empty */
	if(USART_GetITStatus(BSP_SERIAL_PORT, USART_IT_TXE) != RESET) {
		bsp_SerialIrqTxHandler();
	}
}

/**
 * \brief	Initialize the UART for the user communication.
 * 			The UART will be configured with the following settings:
 * 			(+) 115200 baud
 * 			(+) 8 data bit
 * 			(+) one stop bit
 * 			(+) without a parity check bit
 * 			(+) without flow control
 * 			(+) RX/TX interrupts
 * \TODO	Check if USART_Cmd() is necessary.
 */
void bsp_SerialInit(void) {
	uint32_t i;
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	const bsp_gpioconf_t *port[2] = {&BSP_SERIAL_RX, &BSP_SERIAL_TX};

	/* Initialize the GPIOs as inputs. */
	for (i=0; i < 2; i++) {
		/* GPIO Peripheral clock enable */
		RCC_AHB1PeriphClockCmd(port[i]->periph, ENABLE);

		/* GPIO Configuration in alternate function mode */
		GPIO_InitStructure.GPIO_Pin = port[i]->pin;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP ;
		GPIO_Init(port[i]->base, &GPIO_InitStructure);

		/* Connect the alternate function to the pin */
		GPIO_PinAFConfig(port[i]->base,
				BSP_GPIO_PIN_TO_SOURCE(port[i]->pin), port[i]->af);
	}

	/* UART Peripheral clock enable */
	if (BSP_SERIAL_PERIPH == RCC_APB2Periph_USART1 || BSP_SERIAL_PERIPH == RCC_APB2Periph_USART6) {
		RCC_APB2PeriphClockCmd(BSP_SERIAL_PERIPH, ENABLE);
	}
	else {
		RCC_APB1PeriphClockCmd(BSP_SERIAL_PERIPH, ENABLE);
	}

	/* Enable the USART OverSampling by 8 */
	USART_OverSampling8Cmd(BSP_SERIAL_PORT, ENABLE);

	/* Initialize the USART */
	USART_InitStructure.USART_BaudRate = BSP_SERIAL_UART_BAUD;
	USART_InitStructure.USART_WordLength = BSP_SERIAL_UART_LENGTH;
	USART_InitStructure.USART_StopBits = BSP_SERIAL_UART_STOP;
	USART_InitStructure.USART_Parity = BSP_SERIAL_UART_PARITY;
	USART_InitStructure.USART_HardwareFlowControl = BSP_SERIAL_UART_FLOW;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(BSP_SERIAL_PORT, &USART_InitStructure);

	/* Enable the USART interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = BSP_SERIAL_IRQ_CHANEL;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = BSP_SERIAL_IRQ_PRIORITY;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	/* Enable the UART transmit interrupt: this interrupt is generated when
	 * the UART transmit data register is empty.
	 * This is disabled until a string must be send. */
	USART_ITConfig(BSP_SERIAL_PORT, USART_IT_TXE, DISABLE);

	/* Enable the UART receive interrupt: this interrupt is generated when
	 * the UART receive data register not empty. */
	USART_ITConfig(BSP_SERIAL_PORT, USART_IT_RXNE, ENABLE);

	/* Enable UART */
	USART_Cmd(BSP_SERIAL_PORT, ENABLE);
}

/**
 * \brief		Transmit a single byte over the UART.
 * \param[in]	data Byte to transmit.
 */
void bsp_SerialSend(uint16_t data) {
	USART_SendData(BSP_SERIAL_PORT, (data&0xFF));
}

/**
 * \brief		Receive a single byte from the UART.
 * \param[out]	data Received byte.
 */
void bsp_SerialReceive(uint16_t *data) {
	*data = (USART_ReceiveData(BSP_SERIAL_PORT) & 0xFF);
}

/**
 * \brief	Enable the TX interrupt.
 * 			This function is used, if chars are available to send.
 */
void bspSerialTxIrqEnable(void) {
	USART_ITConfig(BSP_SERIAL_PORT, USART_IT_TXE, ENABLE);
}

/**
 * \brief	Disable the TX interrupt.
 * 			This function is used, if no sending chars are available.
 */
void bspSerialTxIrqDisable(void) {
	USART_ITConfig(BSP_SERIAL_PORT, USART_IT_TXE, DISABLE);
}

/**
 * @}
 */

/**
 * @}
 */
