/**
 * \file        bsp_spi.c
 * \brief       Supports the SPI for the communication with the TDC GP22.
 * \date        2014-04-15
 * \version     0.1
 * \author		Kevin Gerber
 *
 * \addtogroup  bsp
 * @{
 *
 * \addtogroup  bsp_spi
 * @{
 */

#include "bsp.h"
#include "bsp_spi.h"

extern void bsp_SPIIrqRxHandler(void);
extern void bsp_SPIIrqTxHandler(void);

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

/**
 * \brief	Initialize all hardware components.
 */
void bsp_SPIInit(void) {
	uint32_t i;
	SPI_InitTypeDef SPI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	/* Configure the chip select */
	bsg_GpioInit(&BSP_SPI_CS_GP22);
	/* Chip must be deselected */
	bsp_GP22ChipDeselect();

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
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
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
void bsp_GP22ChipSelect(void) {
	GPIO_ResetBits(BSP_SPI_CS_GP22.base, BSP_SPI_CS_GP22.pin);
}

/**
 * \brief	Deselect the GP22 chip.
 */
void bsp_GP22ChipDeselect(void) {
	GPIO_SetBits(BSP_SPI_CS_GP22.base, BSP_SPI_CS_GP22.pin);
}


/**
 * \brief		Transmit a single byte over the SPI.
 * \param[in]	data Byte to transmit.
 */
void bsp_SPISend(uint16_t data) {
	SPI_I2S_SendData(BSP_SPI_PORT, (data&0xFF));
}

/**
 * \brief		Receive a single byte from the SPI.
 * \param[out]	data Received byte.
 */
void bsp_SPIReceive(uint16_t *data) {
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
 * @}
 */

/**
 * @}
 */
