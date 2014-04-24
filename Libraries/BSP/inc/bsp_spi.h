/**
 * \file        bsp_spi.h
 * \brief       Supports the SPI for the communication with the TDC GP22.
 * \date        2014-04-24
 * \version     0.2
 * \author		Kevin Gerber
 *
 * \addtogroup  bsp
 * @{
 *
 * \addtogroup  bsp_spi
 * \brief		Supports all functions for the communication with the SPI interface.
 * 				It supports multiple chips over the same SPI interface. The controller
 * 				works as a master.
 * @{
 */

#ifndef BSP_SPI_H_
#define BSP_SPI_H_

/**
 * \typedef	bsp_spicallback_t
 * \brief	Interrupt callback function to manage the received data.
 */
typedef void (*bsp_spicallback_t)(uint8_t *rx_data, uint8_t size);


/*
 * ----------------------------------------------------------------------------
 * Buffer configurations
 * ----------------------------------------------------------------------------
 */
#define BSP_SPI_BUFSIZE_DATA		5			/*!< Maximum number of bytes of a transmission. */
#define BSP_SPI_BUFFER_LEN			(1<<4)		/*!< Transmission buffer size */


/*
 * ----------------------------------------------------------------------------
 * Hardware configurations
 * ----------------------------------------------------------------------------
 */

/**
 * \brief		List off all chip selects, which are supported by the SPI interface.
 * \important	All chip selects require an entry in the \var BSP_SPI_CS array with
 * 				the correct hardware labels.
 */
typedef enum {
	BSP_SPI_CS_CPLD = 0,	/*!< Chip select from the external CPLD. */
	BSP_SPI_CS_ELEMENTCTR	/*!< Counts the chip selects. It must be the last element. */
} bsp_spics_t;

/**
 * \brief	Array off all used LED with their correct hardware label.
 */
static const bsp_gpioconf_t BSP_SPI_CS[] = {
	{RCC_AHB1Periph_GPIOA, GPIOA, GPIO_Pin_4, GPIO_Mode_OUT, GPIO_PuPd_UP},	/* BSP_SPI_CS_GP22 */
};

/**
 * \brief	Hardware label of SPI interface.
 */
static const bsp_gpioconf_t BSP_SPI_PORT_LABEL[] = {
	/* SPI CLK */
	{ RCC_AHB1Periph_GPIOA, GPIOA, GPIO_Pin_5, GPIO_Mode_AF, GPIO_PuPd_DOWN, GPIO_AF_SPI1 },
	/* SPI MISO */
	{ RCC_AHB1Periph_GPIOA, GPIOA, GPIO_Pin_6, GPIO_Mode_AF, GPIO_PuPd_DOWN, GPIO_AF_SPI1 },
	/* SPI MOSI */
	{ RCC_AHB1Periph_GPIOB, GPIOB, GPIO_Pin_5, GPIO_Mode_AF, GPIO_PuPd_DOWN, GPIO_AF_SPI1 }
};

#define BSP_SPI_PORT		SPI1					/*!< Port base address of the SPI port */
#define BSP_SPI_PERIPH		RCC_APB2Periph_SPI1		/*!< RCC AHB peripheral of the SPI port */

#define BSP_SPI_IRQ_CHANEL		SPI1_IRQn			/*!< NVIC UART interrupt */
#define BSP_SPI_IRQ_PRIORITY	0					/*!< NVIC UART interrupt priority */
#define BSP_SPI_IRQ_Handler		SPI1_IRQHandler		/*!< NVIC interrupt handler */


/*
 * ----------------------------------------------------------------------------
 * Prototypes
 * ----------------------------------------------------------------------------
 */
extern void bsp_SPIInit(void);
extern uint8_t bsp_SPITransmit(bsp_spics_t chip, uint8_t *data, uint8_t len, bsp_spicallback_t calback);

#endif /* BSP_GP22_H_ */

/**
 * @}
 */

/**
 * @}
 */
