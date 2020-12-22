/*
 * GL_vars.h
 *
 *  Created on: 20 November 2020
 *      Author: Fedi Salhi
 */

#ifndef HEADER_GL_VARS_H_
#define HEADER_GL_VARS_H_


#include "modified_types.h"
#include "simple_communication.h"
#include "structures_definition.h"

#define MAX_TX_BUFFER_SIZE 128


extern Heart_rate_masurement_t GL_heart_rate_measurement_t;

/* Communication UART channel */
extern Serial_uart_channel_t GL_serial_uart_channel_t;


/* transmit ring buffer */
extern UINT8_t GL_tx_buffer_u8[MAX_TX_BUFFER_SIZE];
extern UINT32_t GL_tx_buffer_first_element_u8 ;
extern UINT32_t GL_tx_buffer_last_element_u8 ;





#endif /* HEADER_GL_VARS_H_ */
