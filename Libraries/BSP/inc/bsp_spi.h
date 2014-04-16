/**
 * \file        bsp_spi.h
 * \brief       Supports the SPI for the communication with the TDC GP22.
 * \date        2014-04-15
 * \version     0.1
 * \author		Kevin Gerber
 *
 * \addtogroup  bsp
 * @{
 *
 * \addtogroup  bsp_spi
 * \brief		Supports all functions for the communication with the TDC GP22.
 * @{
 */

#ifndef BSP_SPI_H_
#define BSP_SPI_H_


/*
 * ----------------------------------------------------------------------------
 * Hardware configurations
 * ----------------------------------------------------------------------------
 */

/**
 * \brief	Hardware label of the GP22 chip select.
 */
static const bsp_gpioconf_t BSP_SPI_CS_GP22 = {
		RCC_AHB1Periph_GPIOA, GPIOA, GPIO_Pin_4, GPIO_Mode_OUT, GPIO_PuPd_NOPULL
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
 * Prototyps
 * ----------------------------------------------------------------------------
 */
extern void bsp_SPIInit(void);
extern void bsp_GP22ChipSelect(void);
extern void bsp_GP22ChipDeselect(void);
extern void bsp_SPISend(uint16_t data);
extern void bsp_SPIReceive(uint16_t *data);
extern void bsp_SPITxIrqEnable(void);
extern void bsp_SPITxIrqDisable(void);

#endif /* BSP_GP22_H_ */

/**
 * @}
 */

/**
 * @}
 */
