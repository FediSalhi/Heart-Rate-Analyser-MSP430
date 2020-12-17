/*
 * simple_communication.h
 *
 *  Created on: 20 November 2020
 *      Author: Fedi Salhi
 */

#ifndef HEADER_SIMPLE_COMMUNICATION_H_
#define HEADER_SIMPLE_COMMUNICATION_H_

#include "modified_types.h"
#include "structures_definition.h"




#define SYNCHRONOUS1            0x11
#define SYNCHRONOUS2            0x99
#define MAX_DATA_LENGTH         255

/* source/target devices */
#define PC                      0x00
#define MSP430                  0x01


/* simple communication packet structure */
typedef struct {
    UINT8_t synchronous_1_u8;
    UINT8_t synchronous_2_u8;
    UINT8_t source_device_u8;
    UINT8_t target_device_u8;
    UINT8_t paket_type_t;
    UINT32_t data_length_u8;
    UINT8_t data_u8[MAX_DATA_LENGTH];
}Simple_communication_paket_t;

typedef enum {
    CAPTURING_SYNCHRONOUS_1,
    CAPTURING_SYNCHRONOUS_2,
    CAPTURING_SOURCE_DEVICE,
    CAPTURING_TARGET_DEVICE,
    CAPTURING_PACKET_TYPE,
    CAPTURING_DATA_LENGTH,
    CAPTURING_DATA
}Packet_capturing_status_e;


typedef struct {
    Packet_capturing_status_e paket_capturing_status_t;
    UINT16_t captured_packet_number_u16;
    UINT16_t solved_packet_number_u16;
    Simple_communication_paket_t simple_com_packet_t;
    void (*packet_solver)(Simple_communication_paket_t*);
}Serial_uart_channel_t;


/* packet types */
typedef enum {
    HEART_RATE_MEASUREMENT_PACKET
}Packet_types_e;



void cont_heart_rate_measurement_packet_create(Simple_communication_paket_t *packet_to_be_sent_t,
                                               const Heart_rate_masurement_t GL_heart_rate_measurement_t );

void cont_heart_rate_measurement_packet_solve(const Simple_communication_paket_t* paket_t,
                                              Heart_rate_masurement_t* heart_rate_measurement_t);
void uart_channel_init();
void simple_communication_package(Simple_communication_paket_t* packet_to_be_sent_t);
void float64_seperate(UINT8_t* data_u8, UINT8_t* index_u8, FLOAT64_t* data_f64);

#endif /* HEADER_SIMPLE_COMMUNICATION_H_ */
