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


/************** Global Variables ********/
uint16_t GL_data = 0;




/*************** prototypes *************/

void adc_init();
void measurement_loop(Heart_rate_masurement_t* heart_rate_measurement_t);

/************ prototypes END ************/



void main() {

    /* Hold Watchdog Timer */
    WDT_A_hold(WDT_A_BASE);

    /* unlock pins */
    PMM_unlockLPM5();

    /* Init ADC */
    adc_init(); //to be implemented


    __bis_SR_register(GIE);
    while(1) {
        /* infinite loop */

        ADC12_B_startConversion(ADC12_B_BASE,
                                ADC12_B_START_AT_ADC12MEM0,
                                ADC12_B_SINGLECHANNEL);

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


    /* memory settings */
    ADC12_B_configureMemoryParam mem_param      = {0};
    mem_param.memoryBufferControlIndex          = ADC12_B_MEMORY_0;
    mem_param.inputSourceSelect                 = ADC12_B_INPUT_A2;
    mem_param.refVoltageSourceSelect            = ADC12_B_VREFPOS_AVCC_VREFNEG_VSS;
    mem_param.endOfSequence                     = ADC12_B_NOTENDOFSEQUENCE;
    mem_param.windowComparatorSelect            = ADC12_B_WINDOW_COMPARATOR_DISABLE;
    mem_param.differentialModeSelect            = ADC12_B_DIFFERENTIAL_MODE_DISABLE;

    ADC12_B_init(ADC12_B_BASE,
                 &param);

    /* use adc in low power mode */
    ADC12_B_setAdcPowerMode(ADC12_B_BASE,
                            ADC12_B_LOWPOWERMODE);

    /* enabling the ADC */
    ADC12_B_enable(ADC12_B_BASE);

    /* set up sampling time */
    ADC12_B_setupSamplingTimer(ADC12_B_BASE,
                               ADC12_B_CYCLEHOLD_16_CYCLES,
                               ADC12_B_CYCLEHOLD_4_CYCLES,
                               ADC12_B_MULTIPLESAMPLESDISABLE);

    /* configure memory buffer */
    ADC12_B_configureMemory(ADC12_B_BASE,
                            &mem_param);

    /* clear interrupt flag */
    ADC12_B_clearInterrupt(ADC12_B_BASE,
                           2,
                           ADC12_B_IFG0); //Mem0

    /* enable memory buffer 0 interrupt */
    ADC12_B_enableInterrupt(ADC12_B_BASE,
                            ADC12_B_IE0,
                            0,
                            0);

    /* start sampling and conversion */
    ADC12_B_startConversion(ADC12_B_BASE,
                            ADC12_B_START_AT_ADC12MEM0,
                            ADC12_B_SINGLECHANNEL);

    GL_data = ADC12_B_getResults(ADC12_B_BASE,
                                  ADC12_B_MEMORY_0);

}









void measurement_loop(Heart_rate_masurement_t* heart_rate_measurement_t) {

    FLOAT64_t measurement_f64 = 0;

    //while (get_adc_state() != CONVERSION_COMPLETED);

    //measurement_f64 = adc_read();

    heart_rate_measurement_t->heart_rate_measurement_mV_f64 = measurement_f64;

}


#pragma vector = ADC12_B_VECTOR
__interrupt void myADC_ISR (void)
{
    switch(ADC12IV)
    {
    case 12:
        GL_data = ADC12_B_getResults(ADC12_B_BASE,
                                      ADC12_B_MEMORY_0);

        __bic_SR_register_on_exit(LPM0_bits);

        break;
    }
}





