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
 * \brief		List off all LEDs to use in software.
 * \important	All LEDs require an entry in the \var port_led array with the correct hardware labels.
 */
typedef enum {
	BSP_SPI_CS_GP22 = 0,	/*!< The green LED identification number. */
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
 * Interrupt buffer
 * ----------------------------------------------------------------------------
 */
#define BSP_SPI_BUFSIZE_DATA		5
#define BSP_SPI_BUFFER_LEN			(1<<4)

/**
 * \typedef	bsp_spicallback_t
 * \brief	Interrupt callback function to manage the received data.
 */
typedef void (*bsp_spicallback_t)(uint8_t *rx_data, uint8_t size);

/**
 * \brief	SPI transfer element.
 * \note	A static data type.
 */
typedef struct {
	bsp_spicallback_t callback;
	uint8_t data[BSP_SPI_BUFSIZE_DATA];
	uint8_t size;
	uint8_t send_ptr;
	uint8_t receive_prt;
	bsp_spics_t chip;
} spitrans_t;

/**
 * \brief	Circular buffer structure.
 * \note	Should be a static data type!
 */
typedef struct {
	uint32_t read;				/*!< TX buffer start index (reading) */
	uint32_t write;				/*!< TX Buffer end index (writing) */
	spitrans_t buffer[BSP_SPI_BUFFER_LEN];	/*!< SPI buffer storage */
	uint8_t sending;;			/*!< TX transmission is pending */
} spi_circbuff_t;


/*
 * ----------------------------------------------------------------------------
 * Prototypes
 * ----------------------------------------------------------------------------
 */
extern void bsp_SPIInit(void);
extern void bsp_SPIChipSelect(bsp_spics_t chip);
extern void bsp_SPIChipDeselect(bsp_spics_t chip);
extern void bsp_SPISend(uint16_t data);
extern void bsp_SPIReceive(uint16_t *data);
extern void bsp_SPITxIrqEnable(void);
extern void bsp_SPITxIrqDisable(void);

extern uint8_t bsp_SPITransmit(bsp_spics_t chip, uint8_t *data, uint8_t len, bsp_spicallback_t calback);

#endif /* BSP_GP22_H_ */

/**
 * @}
 */

/**
 * @}
 */
