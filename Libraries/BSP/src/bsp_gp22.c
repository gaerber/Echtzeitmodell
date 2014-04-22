/**
 * \file        bsp_gp22.c
 * \brief       Supports all functions to manage the TDC GP22.
 * \date        2014-04-15
 * \version     0.1
 * \author		Kevin Gerber
 *
 * \addtogroup  bsp
 * @{
 *
 * \addtogroup  bsp_gp22
 * @{
 */

#include <stdlib.h>

#include "bsp.h"
#include "bsp_spi.h"
#include "bsp_gp22.h"

static uint8_t spi_Get(simple_t *element);

/**
 * Static structure for the sending SPI command.
 */
static simple_t g_SpiElement = {0};

static spi_cricbuf_t g_CircularBuffer;

/*
 * ----------------------------------------------------------------------------
 * SPI interrupt Handler
 * ----------------------------------------------------------------------------
 */

/**
 * \brief	SPI RX interrupt handler. It will be called by the board support package.
 */
void bsp_SPIIrqRxHandler(void) {
	uint16_t dummy;

	bsp_SPIReceive(&dummy);
	g_SpiElement.data[g_SpiElement.receive_prt++] = (uint8_t) dummy;

	if (g_SpiElement.receive_prt == g_SpiElement.size) {
		if (g_SpiElement.irq_callback_handler) {
			uint8_t i;
			uint32_t temp;

			for (i=0; i<g_SpiElement.size; i++) {
				temp = temp << 8;
				temp = temp | g_SpiElement.data[i];
			}
			g_SpiElement.irq_callback_handler(temp);
		}
		g_SpiElement.pending = 0;
		bsp_SPIChipDeselect(BSP_SPI_CS_GP22);
	}
}

/**
 * \brief	SPI TX interrupt handler. It will be called by the board support package.
 */
void bsp_SPIIrqTxHandler(void) {
	/* Check if more data are aviable to send */
	if (g_SpiElement.send_ptr < g_SpiElement.size) {
		/* Send more data */
		bsp_SPISend((uint8_t) g_SpiElement.data[g_SpiElement.send_ptr++]);
	}
	else {
		/* Transmission finished */
		//bsp_GP22ChipDeselect();
		if (spi_Get(&g_SpiElement)) {
			bsp_SPIChipSelect(BSP_SPI_CS_GP22);
			bsp_SPISend((uint8_t) g_SpiElement.data[g_SpiElement.send_ptr++]);
		}
		else {
			bsp_SPITxIrqDisable();
		}
	}
}

/**
 * \brief	Allocate and preconfig a SPI block.
 * \return	Pointer to the allocated block. NULL if no space are available.
 */
static simple_t* allocateSpiElement(void) {
	simple_t* new_spi_element = NULL;

	if (!g_SpiElement.pending) {
		g_SpiElement.pending = 1;
		g_SpiElement.irq_callback_handler = NULL;
		g_SpiElement.receive_prt = 0;
		g_SpiElement.send_ptr = 0;

		new_spi_element = &g_SpiElement;
	}

	return new_spi_element;
}

static void prepareSpiElement(simple_t *element) {
	element->pending = 1;
	element->irq_callback_handler = NULL;
	element->receive_prt = 0;
	element->send_ptr = 0;
}

static uint8_t spi_Put(simple_t *element) {
	uint8_t success = 0;

	/* Check if space is available in the circular buffer */
	if (g_CircularBuffer.read + (SPI_BUFFER_LEN - 2) != g_CircularBuffer.write) {

		/* Put the character into the circular buffer */
		g_CircularBuffer.buffer[g_CircularBuffer.write++ & (SPI_BUFFER_LEN-1)] = *element;

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

static uint8_t spi_Get(simple_t *element) {
	uint8_t success = 0;

	/* Checks if a character is available */
	if (g_CircularBuffer.read != g_CircularBuffer.write) {
		/* Gets the next character */
		*element = g_CircularBuffer.buffer[g_CircularBuffer.read++ & (SPI_BUFFER_LEN-1)];
		success = 1;
	}

	return success;
}

/*
 * ----------------------------------------------------------------------------
 * Functions
 * ----------------------------------------------------------------------------
 */

/**
 * \brief	Initialize the GP22.
 */
void bsp_GP22Init(void) {
	/* Requires the SPI */
	bsp_SPIInit();

	/* Reset circular buffer */
	g_CircularBuffer.read = g_CircularBuffer.write;

	/* ALU interrupt as a GPIO input */

	/* Sets all Register */
}

uint8_t bsp_GP22RegWirte_V1(gp22_opcode_t opcode, uint32_t data) {
	uint8_t success = 0;
	simple_t *spi_element;

	spi_element = allocateSpiElement();

	if (spi_element) {
		/* Set the size and data */
		g_SpiElement.size = 4;
		(*g_SpiElement.data) = data;

		/* the TX interrupt must be enabled to start the transmission */
		bsp_SPITxIrqEnable();
		/* Transmission started */
		success = 1;
	}

	return success;
}

uint8_t bsp_GP22RegWirte(gp22_opcode_t opcode, uint32_t data) {
	simple_t spi_element;

	prepareSpiElement(&spi_element);

	/* Set the size and data */
	spi_element.size = 4;
	(*spi_element.data) = data;

	return spi_Put(&spi_element);
}


uint8_t bsp_GP22SendOpcode(gp22_opcode_t opcode) {
	simple_t spi_element;

	prepareSpiElement(&spi_element);

	/* Set the size and data */
	spi_element.size = 1;
	spi_element.data[0] = opcode;

	return spi_Put(&spi_element);
}
uint8_t bsp_GP22ReadBlocked(gp22_opcode_t opcode, uint8_t *data, uint8_t bytes_ctr) {

}
uint8_t bsp_GP22ReadIrqCallback(gp22_opcode_t opcode, uint8_t bytes_ctr,
		void (*irq_callback_handler)(uint32_t) ) {

}

/**
 * @}
 */

/**
 * @}
 */
