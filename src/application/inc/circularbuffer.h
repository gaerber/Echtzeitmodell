/**
 * \file        curcularbuffer.c
 * \brief		The circular buffer manages the UART hardware with the interrupts.
 * \date        2014-04-07
 * \version     2.0.1
 *
 * \addtogroup	Circular Buffer
 * \brief		The circular buffer manages the UART hardware. All interrupt will
 * 				be handled within this module. All in- and outcoming character were
 * 				stored in the circular buffer.
 * \{
 */

#ifndef CIRCULARBUFFER_H_
#define CIRCULARBUFFER_H_

#include <stdint.h>

#define	TX_BUFFER_LEN		(1<<8)		/*!< TX buffer storage size */
#define	RX_BUFFER_LEN		(1<<8)		/*!< RX buffer storage size */

/** User defined block function */
#define BLOCK_UNTIL_SPACE_AVAIABLE()

/** User defined function called when a new character is available */
#define NEW_CHRACTER_AVAILABLE()

/*
 * -----------------------------------------------------------------------
 * Prototypes
 * -----------------------------------------------------------------------
 */

/**
 * \brief	Circular buffer structure.
 * \note	Should be a static data type!
 */
typedef struct {
	uint32_t tx_read;				/*!< TX buffer start index (reading) */
	uint32_t tx_write;				/*!< TX Buffer end index (writing) */
	char tx_buffer[TX_BUFFER_LEN];	/*!< TX buffer storage */
	uint8_t tx_sending;				/*!< TX transmission is pending */
	uint32_t rx_read;				/*!< RX buffer start index (reading) */
	uint32_t rx_write;				/*!< RX buffer ende index (writing) */
	char rx_buffer[RX_BUFFER_LEN];	/*!< RX buffer storage */
} circbuff_t;


/*
 * -----------------------------------------------------------------------
 * Functions prototype
 * -----------------------------------------------------------------------
 */
extern void CircularBufferInit(void);
extern uint8_t CircularBufferCharPut(char a);
extern uint8_t CircularBufferCharGet(char *a);
extern uint32_t CircularBufferStringPut(char *string, uint32_t length);

#endif /* CIRCULARBUFFER_H_ */
