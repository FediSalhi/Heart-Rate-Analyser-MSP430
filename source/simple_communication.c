/*
 * simple_communication.c
 *
 *  Created on: 20 November 2020
 *      Author: Fedi Salhi
 */

#include "modified_types.h"
#include "simple_communication.h"
#include "structures_definition.h"
#include "GL_vars.h"
#include <msp430.h>


void cont_heart_rate_measurement_packet_create(Simple_communication_paket_t *packet_to_be_sent_t,
                                               const Heart_rate_masurement_t GL_heart_rate_measurement_t )
{
    UINT8_t index = 0;

    float64_seperate(packet_to_be_sent_t->data_u8, &index, &GL_heart_rate_measurement_t.heart_rate_measurement_mV_f64);

    packet_to_be_sent_t->data_length_u8 = index;
    packet_to_be_sent_t->paket_type_t = HEART_RATE_MEASUREMENT_PACKET;
}



void cont_heart_rate_measurement_packet_solve(const Simple_communication_paket_t* paket_t,
                                              Heart_rate_masurement_t* heart_rate_measurement_t) {

    /* uint birlestir fonk. */
   __no_operation();
}

void solve_packets(Simple_communication_paket_t* packet)
{

}

void uart_channel_init()
{
    GL_serial_uart_channel_t.paket_capturing_status_t       = CAPTURING_SYNCHRONOUS_1;
    GL_serial_uart_channel_t.captured_packet_number_u16     = 0;
    GL_serial_uart_channel_t.solved_packet_number_u16       = 0;
    //GL_serial_uart_channel_t.Simple_communication_paket_t   = {0};
    GL_serial_uart_channel_t.packet_solver                  = &solve_packets;
}

void simple_communication_package(Simple_communication_paket_t* packet_to_be_sent_t)
{
    packet_to_be_sent_t->synchronous_1_u8 = SYNCHRONOUS1;
    packet_to_be_sent_t->synchronous_2_u8 = SYNCHRONOUS2;
    packet_to_be_sent_t->source_device_u8 = MSP430;
    packet_to_be_sent_t->target_device_u8 = PC;
}


void float64_seperate(UINT8_t* data_u8,
                      UINT8_t* index_u8,
                      const FLOAT64_t* data_f64)
{
    data_u8[0]  = ((UINT8_t*)data_f64)[0];
    data_u8[1]  = ((UINT8_t*)data_f64)[1];
    data_u8[2]  = ((UINT8_t*)data_f64)[2];
    data_u8[3]  = ((UINT8_t*)data_f64)[3];
    data_u8[4]  = ((UINT8_t*)data_f64)[4];
    data_u8[5]  = ((UINT8_t*)data_f64)[5];
    data_u8[6]  = ((UINT8_t*)data_f64)[6];
    data_u8[7]  = ((UINT8_t*)data_f64)[7];

    *index_u8 = 8;
}

void floar64_combine(const UINT8_t* data_u8,
                     FLOAT64_t* data_f64)
{

}

void sent_packet(Simple_communication_paket_t *packet_to_be_sent_t)
{
    int i = 0;
    UCA1TXBUF = packet_to_be_sent_t->synchronous_1_u8;
    for(i=0; i<1000; i++) {}
    UCA1TXBUF = packet_to_be_sent_t->synchronous_2_u8;
    for(i=0; i<1000; i++) {}
    UCA1TXBUF = packet_to_be_sent_t->target_device_u8;

    for(i=0; i<100000; i++) {} //bad delay
}



