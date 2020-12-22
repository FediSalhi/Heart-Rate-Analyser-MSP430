/*
 * GL_vars.c
 *
 *  Created on: 20 November 2020
 *      Author: Fedi Salhi
 */


#include "modified_types.h"
#include "simple_communication.h"
#include "structures_definition.h"
#include "GL_vars.h"


Heart_rate_masurement_t GL_heart_rate_measurement_t = {0};

/* Communication UART channel */
Serial_uart_channel_t GL_serial_uart_channel_t;

/* transmit ring buffer */
UINT8_t GL_tx_buffer_u8[MAX_TX_BUFFER_SIZE];
UINT32_t GL_tx_buffer_first_element_u8 = 0;
UINT32_t GL_tx_buffer_last_element_u8  = 0;

