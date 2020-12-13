/*
 * main.c
 *
 *  Created on: 20 November 2020
 *      Author: Fedi Salhi
 */

#include <msp430.h>
#include "GL_vars.h"
#include "simple_communication.h"
#include "structures_definition.h"
#include "driverlib.h"


/*************** prototypes *************/

void adc_init();
void measurement_loop(Heart_rate_masurement_t* heart_rate_measurement_t);

/************ prototypes END ************/



void main() {

    /* Hold Watchdog Timer */
    WDT_A_hold(WDT_A_BASE);

    /* Init ADC */
    adc_init(); //to be implemented



    while(1) {
        /* infinite loop */

        measurement_loop(&heart_rate_measurement_t); //TODO: define this func."

       // communication_loop(heart_rate_measurement_t, &Cont_heart_rate_measurement_packet_t); //TODO: define this func


    }

}



void adc_init()
{

    /* setting the ADC */
    ADC12_B_initParam param = {0};
    param.sampleHoldSignalSourceSelect  = ADC12_B_SAMPLEHOLDSOURCE_1;
    param.clockSourceSelect             = ADC12_B_SAMPLEHOLDSOURCE_SC;    //5MHz from UCS
    param.clockSourceDivider            = ADC12_B_CLOCKDIVIDER_1;
    param.clockSourcePredivider         = ADC12_B_CLOCKPREDIVIDER__1;
    param.internalChannelMap            = 0; // no mapping of internal signals

    ADC12_B_init(ADC12_B_BASE,
                 &param);

    /* enabling the ADC */
    ADC12_B_enable(ADC12_B_BASE);

    /* set up sampling time */
    ADC12_B_setupSamplingTimer(ADC12_B_BASE,
                               ADC12_B_CYCLEHOLD_16_CYCLES,
                               ADC12_B_CYCLEHOLD_4_CYCLES,
                               ADC12_B_MULTIPLESAMPLESDISABLE);

    /* configure memory buffer */

}









void measurement_loop(Heart_rate_masurement_t* heart_rate_measurement_t) {

    FLOAT64_t measurement_f64 = 0;

    //while (get_adc_state() != CONVERSION_COMPLETED);

    //measurement_f64 = adc_read();

    heart_rate_measurement_t->heart_rate_measurement_mV_f64 = measurement_f64;

}






