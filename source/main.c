/*
 * main.c
 *
 *  Created on: 20 November 2020
 *      Author: Fedi Salhi
 */

#include<msp430fr5994.h>
#include "GL_vars.h"
#include "simple_communication.h"
#include "structures_definition.h"


/*************** prototypes *************/
void stop_watch_dog_timer();
void adc_init();
ADC12_B_conversion_state_t get_adc_state();
FLOAT64_t adc_read();
void measurement_loop(Heart_rate_masurement_t* heart_rate_measurement_t);

/************ prototypes END ************/



void main() {

    /* initializations */
    adc_init();



    while(1) {
        /* infinite loop */

        measurement_loop(&heart_rate_measurement_t); //TODO: define this func.

       // communication_loop(heart_rate_measurement_t, &Cont_heart_rate_measurement_packet_t); //TODO: define this func


    }

}

void measurement_loop(Heart_rate_masurement_t* heart_rate_measurement_t) {

    FLOAT64_t measurement_f64;

    while (get_adc_state() != CONVERSION_COMPLETED);

    measurement_f64 = adc_read();

    heart_rate_measurement_t->heart_rate_measurement_mV_f64 = measurement_f64;

}

void adc_init(){

    ADC12CTL1 |= ADC12CONSEQ_0;     /* single channel, single conversion */
    ADC12CTL0 |= ADC12ON_1;         /* ADC12_B on */
    ADC12CTL3 |= ADC12CSTARTADD_0;  /* Conversion start address ADC12MEM0 */

    ADC12MCTL0 |= ADC12DIF_0;        /* single ended mode */
    ADC12MCTL0 |= ADC12INCH_0;       /* Input channel A0 */
    ADC12CTL1  |= ADC12SHP_0;        /* SAMPCON signal is sourced from the sample-input signal. */

    ADC12CTL1 |= ADC12SHS_0;        /* ADC12SC selected as sample and hold source */

    __bis_SR_register(GIE+CPUOFF);  /* LPM0 with general interrupt enable */

    ADC12IER0 |= ADC12IE0_1;        /* interrupt enable */

    __delay_cycles(100);



}

ADC12_B_conversion_state_t get_adc_state(){

    ADC12CTL0 |= ADC12ENC_1;    /* ADC12_B enable */

    ADC12CTL0 |= ADC12SC;       /* start conversion */

    if (ADC12IFGR0) {
        /* conversion completed */

        return CONVERSION_COMPLETED;
    }

    else {
        return CONVERSION_NOT_COMPLETED;
    }

}

FLOAT64_t adc_read() {

    FLOAT64_t adc_result;
    adc_result = ADC12MEM0;

    return adc_result;
}




void stop_watch_dog_timer(){

    /* Any write operation to WDTCTL must be a word operation with 05Ah (WDTPW) in the upper byte */
    WDTCTL |= (WDTPW + WDTHOLD);
}


