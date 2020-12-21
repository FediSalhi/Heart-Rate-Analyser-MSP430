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



/************** defines ***************/
#define PRECISION_MICRO_V 830


/************** Global Variables ********/
uint16_t GL_adc12_read_u16 = 0;
FLOAT64_t GL_measurement_mV_f64 = 0;




/*************** prototypes *************/

void adc_init();
void gpio_init();
void uart_init();
void measurement_loop(Heart_rate_masurement_t* GL_heart_rate_measurement_t);
void communication_loop();

/************ prototypes END ************/



void main() {



    /* Hold Watchdog Timer */
    WDT_A_hold(WDT_A_BASE);

    /* unlock pins */
    PMM_unlockLPM5();

    /* init uart communication channel */
    uart_channel_init();

    gpio_init();

    /* Init ADC */
    adc_init();

    /* uart init */
    uart_init();

    __bis_SR_register(GIE);


    int i;
    while(1)
    {
       measurement_loop(&GL_heart_rate_measurement_t);
       communication_loop();


       //UCA1TXBUF = 0x651;
        //for(i=0; i<10000; i++) {} //bad delay

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

void measurement_loop(Heart_rate_masurement_t* GL_heart_rate_measurement_t)
{
    ADC12CTL0 |= ADC12ENC | ADC12SC;

    while ((ADC12IFGR0 & ADC12IFG0) == 0);

    /* read coverted measurement */
    GL_adc12_read_u16 = ADC12MEM0;

    /* convert digital value to voltage */
    GL_measurement_mV_f64 = GL_adc12_read_u16 * 0.807 ;

    GL_heart_rate_measurement_t->heart_rate_measurement_mV_f64 = GL_measurement_mV_f64;
}

void communication_loop()
{
    Simple_communication_paket_t packet_to_be_sent_t = {0};
    cont_heart_rate_measurement_packet_create(&packet_to_be_sent_t,
                                              GL_heart_rate_measurement_t );

    simple_communication_package(&packet_to_be_sent_t);

    sent_packet(&packet_to_be_sent_t);
}

void uart_init(void){
    P2SEL1 |= BIT5 + BIT6;              //Activate Pin for UART use
    P2SEL0 &= ~BIT5 + ~BIT6;            //Activate Pin for UART use

    UCA1CTLW0 |= UCSSEL_2;              //Select clock SMCLK

    UCA1BRW = 0x6;                      //Set Baud rate 9600 : UCA1BRW = INT(F_CPU/BAUD_soll) =INT(1MHz/9600) = 104 with oversampling: 6
    UCA1MCTLW |= UCBRS5 + UCOS16 + UCBRF3;     //Modulation according to datasheet table: UCBRS = 0x20 = b100000 and UCOS16 = 1 and UCBRF = 8 = 0x8 = b1000

    UCA1CTLW0 &= ~UCSWRST;  //Reset UART module
 }







#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)

 #pragma vector=USCI_A1_VECTOR
 __interrupt void USCI_A1_ISR(void)

 #elif defined(__GNUC__)
 void __attribute__ ((interrupt(USCI_A1_VECTOR))) USCI_A1_ISR (void)
 #else
 #error Compiler not supported!
 #endif
 {
     switch(__even_in_range(UCA1IV, USCI_UART_UCTXCPTIFG))
     {
         case USCI_NONE:
             break;
         case USCI_UART_UCRXIFG:
             while(!(UCA1IFG & UCTXIFG));
             UCA1TXBUF = UCA1RXBUF;        //echo
             P1OUT ^= ~BIT0;               //LED P1.0 switched off
             __no_operation();
             break;
         case USCI_UART_UCTXIFG:
             break;
         case USCI_UART_UCSTTIFG:
             break;
         case USCI_UART_UCTXCPTIFG:
             break;
     }
}

