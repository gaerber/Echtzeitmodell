/**
 * \file        curcularbuffer.c
 * \brief		The circular buffer manages the UART hardware with the interrupts.
 * \date        2014-04-07
 * \version     2.0.1
 *
 * \addtogroup	Circular Buffer
 * \{
 */

#include "circularbuffer.h"
#include "bsp_serial.h"

/*
* -----------------------------------------------------------------------
* private variables
* -----------------------------------------------------------------------
*/

/**
 * \brief	Circular buffer manager.
 */
circbuff_t g_CircularBuffer;

/*
* -----------------------------------------------------------------------
* Interrupt functions
* -----------------------------------------------------------------------
*/

/**
 * \brief	UART TX interrupt handler. It will be called by the board support package.
 */
void bsp_SerialIrqTxHandler(void) {
	/* Check if character are available to send */
	if (g_CircularBuffer.tx_read != g_CircularBuffer.tx_write) {
		/* Send the next character */
		bsp_SerialSend((uint16_t) g_CircularBuffer.tx_buffer[g_CircularBuffer.tx_read++ & (TX_BUFFER_LEN-1)]);
		g_CircularBuffer.tx_sending = 1;
	}
	else {
		/* There are no more character to send */
		g_CircularBuffer.tx_sending = 0;
		/* Disable the TX interrupt */
		bspSerialTxIrqDisable();
	}
}

/**
 * \brief	UART RX interrupt handler. It will be called by the board support package.
 */
void bsp_SerialIrqRxHandler(void) {
	uint16_t c;

	/* Get the received character from the hardware */
	bsp_SerialReceive(&c);

	/* Check if the RX circular buffer is not full */
	if (g_CircularBuffer.rx_read + RX_BUFFER_LEN != g_CircularBuffer.rx_write) {
		/* Store the incoming character in the circular buffer */
		g_CircularBuffer.rx_buffer[g_CircularBuffer.rx_write++ & (RX_BUFFER_LEN-1)] = (char) c;
	}
	/* Else: If no space is available, the character will be losing */
}


/*
* -----------------------------------------------------------------------
* Functions
* -----------------------------------------------------------------------
*/

/**
* \brief	Initialize the hardware UART and the circular buffer.
*/
void CircularBufferInit(void) {
	/* Initialize the hardware */
	bsp_SerialInit();

	/* Reset the circular buffer */
	g_CircularBuffer.rx_read = g_CircularBuffer.rx_write;
	g_CircularBuffer.tx_read = g_CircularBuffer.tx_write;
}

/**
 * \brief	Puts a character into the circular buffer.
 * \param	a is the character, which will put into the circular buffer.
 * \return	True if the character was put into the circular buffer, otherwise false.
 */
uint8_t CircularBufferCharPut(char a) {
	uint8_t success = 1;

	/* Check if space is available in the circular buffer */
	if (g_CircularBuffer.tx_read + (TX_BUFFER_LEN - 2) != g_CircularBuffer.tx_write) {

		/* Put the character into the circular buffer */
		g_CircularBuffer.tx_buffer[g_CircularBuffer.tx_write++ & (TX_BUFFER_LEN-1)] = a;

		/* If the circular buffer is not sending, the TX interrupt must be enabled
		 * to start the transmission */
		if (g_CircularBuffer.tx_sending == 0) {
			/* Enable the TX interrupt */
			bspSerialTxIrqEnable();
		}
		success = 1;
	}
	else {
		success = 0;
	}

	return success;
}

/**
 * \brief	Reads a character from the circular buffer and gives it to the user.
 * \param	a Reference to the character storage.
 * \return	False if no character is available in the circular buffer.
 */
uint8_t CircularBufferCharGet(char *a) {
	uint8_t success;

	/* Checks if a character is available */
	if (g_CircularBuffer.rx_read != g_CircularBuffer.rx_write) {
		/* Gets the next character */
		*a = g_CircularBuffer.rx_buffer[g_CircularBuffer.rx_read++ & (RX_BUFFER_LEN-1)];
		success = 1;
	}
	else {
		success = 0;
	}

	return success;
}

#if 0
/**
* \brief Prueft ob sich noch Zeichen im RX Ringbuffer befinden.
* \return TRUE falls noch Zeichen vorhanden sind, sonst FALSE.
*/
BOOL UART_CharAviable(void) {
	if (s_RingBuffer.rx_read != s_RingBuffer.rx_write)
		return TRUE;
	return FALSE;
}

/**
* \brief Prueft ob noch ein Zeichen im TX Ringbuffer platz hat.
* \return TRUE falls noch Zeichen platz haben, sonst FALSE.
*/
BOOL UART_SpaceAviable(void) {
	if (s_RingBuffer.tx_read + (TX_BUFFER_LEN - 2) != s_RingBuffer.tx_write)
		return TRUE;
	return FALSE;
}

/**
* \brief Loescht den RX Ringbuffer.
* \return Nichts.
*/
void UART_RxClr(void) {
	s_RingBuffer.rx_read = s_RingBuffer.rx_write;
}

#endif

/**
* \}
*/
