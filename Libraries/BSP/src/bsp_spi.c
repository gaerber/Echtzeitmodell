/**
 * \file        bsp_spi.c
 * \brief       Supports the SPI for the communication with the TDC GP22.
 * \date        2014-04-24
 * \version     0.2
 * \author		Kevin Gerber
 *
 * \addtogroup  bsp
 * @{
 *
 * \addtogroup  bsp_spi
 * @{
 */

#include <string.h>

#include "bsp.h"
#include "bsp_spi.h"

/*
 * ----------------------------------------------------------------------------
 * Private data types
 * ----------------------------------------------------------------------------
 */

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
 * Local variables
 * ----------------------------------------------------------------------------
 */
static spi_circbuff_t g_CircularBuffer;
static spitrans_t g_SpiElement;


/*
 * ----------------------------------------------------------------------------
 * Private functions prototypes
 * ----------------------------------------------------------------------------
 */
void bsp_SPIIrqRxHandler(void);
void bsp_SPIIrqTxHandler(void);
void bsp_SPIChipSelect(bsp_spics_t chip);
void bsp_SPIChipDeselect(bsp_spics_t chip);
void bsp_SPISendByte(uint16_t data);
void bsp_SPIReceiveByte(uint16_t *data);
void bsp_SPITxIrqEnable(void);
void bsp_SPITxIrqDisable(void);


/*
 * ----------------------------------------------------------------------------
 * Interrupt Handler
 * ----------------------------------------------------------------------------
 */

/**
 * \brief	SPI RX interrupt handler. It will be called by the board support package.
 */
void bsp_SPIIrqRxHandler(void) {
	uint16_t dummy;

	bsp_SPIReceiveByte(&dummy);
	g_SpiElement.data[g_SpiElement.receive_prt++] = (uint8_t) dummy;

	if (g_SpiElement.receive_prt == g_SpiElement.size) {
		if (g_SpiElement.callback) {
			g_SpiElement.callback(g_SpiElement.data, g_SpiElement.size);
		}
		bsp_SPIChipDeselect(g_SpiElement.chip);
		g_CircularBuffer.sending = 0;
	}
}

/**
 * \brief	SPI TX interrupt handler. It will be called by the board support package.
 */
void bsp_SPIIrqTxHandler(void) {
	if (g_CircularBuffer.sending == 0) {
		/* Check if data are available to send */
		if (g_CircularBuffer.read != g_CircularBuffer.write) {
			/* Get the SPI Element */
			g_SpiElement = g_CircularBuffer.buffer[g_CircularBuffer.read++ & (BSP_SPI_BUFFER_LEN-1)];
			bsp_SPIChipSelect(g_SpiElement.chip);
			g_CircularBuffer.sending = 1;
		}
	}

	/* Check if more data are aviable to send */
	if (g_SpiElement.send_ptr < g_SpiElement.size) {
		/* Send more data */
		bsp_SPISendByte((uint8_t) g_SpiElement.data[g_SpiElement.send_ptr++]);
	}
	else {
		/* Transmission finished */
		bsp_SPITxIrqDisable();
	}
}

/**
 * \brief	This function handles SPIx global interrupt request.
 */
void BSP_SPI_IRQ_Handler(void) {
	/* SPI receive data register not empty */
	if(SPI_I2S_GetFlagStatus(BSP_SPI_PORT, SPI_I2S_FLAG_RXNE) == SET) {
		bsp_SPIIrqRxHandler();
	}

	/* SPI transmit data register is empty */
	if(SPI_I2S_GetFlagStatus(BSP_SPI_PORT, SPI_I2S_FLAG_TXE) == SET) {
		bsp_SPIIrqTxHandler();
	}
}


/*
 * ----------------------------------------------------------------------------
 * Implementation
 * ----------------------------------------------------------------------------
 */

/**
 * \brief	Initialize all hardware components.
 */
void bsp_SPIInit(void) {
	uint32_t i;
	SPI_InitTypeDef SPI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	/* Initialize all registred chip selects */
	for (i=0; i < BSP_SPI_CS_ELEMENTCTR; i++) {
		/* Initialize all GPIOs in their function */
		bsg_GpioInit(&(BSP_SPI_CS[i]));

		/* Chip must be deselected */
		bsp_SPIChipDeselect(i);
	}

	/* SPI GPIO configuration */
	for (i=0; i<3; i++) {
		/* Initialize all GPIOs in their function */
		bsg_GpioInit(&(BSP_SPI_PORT_LABEL[i]));
	}

	/* Enable the SPI clock */
	if (BSP_SPI_PERIPH == RCC_APB1Periph_SPI2 || BSP_SPI_PERIPH == RCC_APB1Periph_SPI3) {
		RCC_APB1PeriphClockCmd(BSP_SPI_PERIPH, ENABLE);
	}
	else {
		RCC_APB2PeriphClockCmd(BSP_SPI_PERIPH, ENABLE);
	}

	/* SPI configuration as master */
	//SPI_I2S_DeInit(BSP_SPI_PORT);
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;//SPI_CPHA_2Edge;
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_32;
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_InitStructure.SPI_CRCPolynomial = 7;
	SPI_Init(BSP_SPI_PORT, &SPI_InitStructure);

	/* Configure the SPI interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = BSP_SPI_IRQ_CHANEL;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = BSP_SPI_IRQ_PRIORITY;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	/* Enable the SPI receive interrupt: this interrupt is generated when
	 * the SPI receive data register not empty. */
	SPI_I2S_ITConfig(BSP_SPI_PORT, SPI_I2S_IT_RXNE, ENABLE);

	/* Enable the SPI transmit interrupt: this interrupt is generated when
	 * the SPI transmit data register is empty.
	 * This is disabled until a byte must be send. */
	SPI_I2S_ITConfig(BSP_SPI_PORT, SPI_I2S_IT_TXE, DISABLE);

	/* Enable the SPI peripheral */
	SPI_Cmd(BSP_SPI_PORT, ENABLE);
}

/**
 * \brief	Select the GP22 chip.
 */
void bsp_SPIChipSelect(bsp_spics_t chip) {
	//GPIO_WriteBit(BSP_SPI_CS[chip].base,BSP_SPI_CS[chip].pin,RESET);
	GPIO_ResetBits(BSP_SPI_CS[chip].base, BSP_SPI_CS[chip].pin);
}

/**
 * \brief	Deselect the GP22 chip.
 */
void bsp_SPIChipDeselect(bsp_spics_t chip) {
	//GPIO_WriteBit(BSP_SPI_CS[chip].base,BSP_SPI_CS[chip].pin,SET);
	GPIO_SetBits(BSP_SPI_CS[chip].base, BSP_SPI_CS[chip].pin);
}


/**
 * \brief		Transmit a single byte over the SPI.
 * \param[in]	data Byte to transmit.
 */
void bsp_SPISendByte(uint16_t data) {
	SPI_I2S_SendData(BSP_SPI_PORT, (data&0xFF));
}

/**
 * \brief		Receive a single byte from the SPI.
 * \param[out]	data Received byte.
 */
void bsp_SPIReceiveByte(uint16_t *data) {
	*data = (SPI_I2S_ReceiveData(BSP_SPI_PORT) & 0xFF);
}

/**
 * \brief	Enable the SPI TX interrupt.
 * 			This function is used, if data are available to send.
 */
void bsp_SPITxIrqEnable(void) {
	SPI_I2S_ITConfig(BSP_SPI_PORT, SPI_I2S_IT_TXE, ENABLE);
}

/**
 * \brief	Disable the SPI TX interrupt.
 * 			This function is used, if no sending datas are available.
 */
void bsp_SPITxIrqDisable(void) {
	SPI_I2S_ITConfig(BSP_SPI_PORT, SPI_I2S_IT_TXE, DISABLE);
}

/**
 * \brief	Send a data block over SPI to a chip.
 * 			This is an non blocking function. The data were buffered and the transfer
 * 			will be in interrupt mode.
 * \param 	chip A reference to the chip, which will be selected during the transmission.
 * \param	data Array of data, to transmit.
 * \param	len Length of the data array. Maximum length is defined in \var BSP_SPI_BUFSIZE_DATA.
 * \param	calback	Callback function to manage the received data. It will be called in the SPI
 * 			interrupt. If no data must be received, set it to \var NULL.
 * \return	TRUE if the data were stored in the buffer, FALSE if no space is available.
 */
uint8_t bsp_SPITransmit(bsp_spics_t chip, uint8_t *data, uint8_t len, bsp_spicallback_t calback) {
	uint8_t success = 0;
	spitrans_t element;

	/* Parameter check */
	assert(chip < BSP_SPI_CS_ELEMENTCTR);
	assert(len <= BSP_SPI_BUFSIZE_DATA);

	/* Check if space is available in the circular buffer */
	if (g_CircularBuffer.read + (BSP_SPI_BUFFER_LEN - 2) != g_CircularBuffer.write) {

		/* Prepare the element */
		element.chip = chip;
		element.send_ptr = 0;
		element.receive_prt = 0;
		memcpy(element.data, data, len * sizeof(uint8_t));
		element.size = len;
		element.callback = calback;

		/* Put the character into the circular buffer */
		g_CircularBuffer.buffer[g_CircularBuffer.write++ & (BSP_SPI_BUFFER_LEN-1)] = element;

		/* If the circular buffer is not sending, the TX interrupt must be enabled
		 * to start the transmission */
		if (g_CircularBuffer.sending == 0) {
			/* Enable the TX interrupt */
			bsp_SPITxIrqEnable();
		}
		success = 1;
	}

	return success;
}

/**
 * @}
 */

/**
 * @}
 */
