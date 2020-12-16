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
uint16_t GL_adc12_read_u16 = 0;
FLOAT64_t GL_measurement_mV_f64 = 0;




/*************** prototypes *************/

void adc_init();
void gpio_init();
void measurement_loop(Heart_rate_masurement_t* heart_rate_measurement_t);

/************ prototypes END ************/



void main() {

    /* Hold Watchdog Timer */
    WDT_A_hold(WDT_A_BASE);

    /* unlock pins */
    PMM_unlockLPM5();

    gpio_init();

    /* Init ADC */
    adc_init(); //to be implemented


    __bis_SR_register(GIE);
    while(1) {

        measurement_loop(&heart_rate_measurement_t); //TODO: define this func."

       // communication_loop(heart_rate_measurement_t, &Cont_heart_rate_measurement_packet_t); //TODO: define this func


    }

}



void adc_init()
{


    /* set conversion clock cycles to 16 */
    ADC12CTL0 |= ADC12SHT02 ;

    /* turn on ADC */
    ADC12CTL0 |= ADC12ON;

    /* chose SMCLK */
    ADC12CTL1 |= ADC12SSEL_2;

    /* sample signal source = sampling timer */
    ADC12CTL1 |= ADC12SHP;

    /* set resolution resolution 12 bits */
    ADC12CTL2 |= ADC12RES_2;


    /* select A2 channel */
    ADC12MCTL0 = ADC12INCH_2;



}

void gpio_init()
{
    /* select P1.2 as analog input */
    P1SEL1 |= BIT2;
    P1SEL0 |= BIT2;

}







void measurement_loop(Heart_rate_masurement_t* heart_rate_measurement_t) {



    ADC12CTL0 |= ADC12ENC | ADC12SC;

    while ((ADC12IFGR0 & ADC12IFG0) == 0);

    /* read coverted measurement */
    GL_adc12_read_u16 = ADC12MEM0;

    /* convert digital value to voltage */
    GL_measurement_mV_f64 =

    heart_rate_measurement_t->heart_rate_measurement_mV_f64 = measurement_f64;

}


#pragma vector = ADC12_VECTOR
__interrupt void myADC_ISR (void)
{
    switch(ADC12IV)
    {
    case 12:
        GL_data = ADC12MEM0;

        __bic_SR_register_on_exit(LPM0_bits);

        break;
    }
}





