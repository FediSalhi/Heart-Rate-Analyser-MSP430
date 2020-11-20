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
/************ prototypes END ************/



void main() {

    /* initializations */
    adc_init();



    while(1) {
        /* infinite loop */

        //measurement_loop(&heart_rate_measurement_t); //TODO: define this func.

       // communication_loop(heart_rate_measurement_t, &Cont_heart_rate_measurement_packet_t); //TODO: define this func


    }

}

void measuremenet_loop(Heart_rate_masurement_t* heart_rate_measurement_t) {

}

void adc_init(){


}

void stop_watch_dog_timer(){

    WDTCTL |= (WDTPW + WDTHOLD);
}


