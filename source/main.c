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
    //PMM_unlockLPM5();

    /* init uart communication channel */
    uart_channel_init();

    gpio_init();

    /* Init ADC */
    adc_init();

    /* uart init */
    uart_init();

    __bis_SR_register(GIE);




    WDTCTL = WDTPW | WDTHOLD; // Stop watchdog timer

    // Set P2.0 and P2.1 for USCI_A0 UART operation
    P2SEL0 &= ~(BIT0 | BIT1);
    P2SEL1 |= BIT0 | BIT1;

    P1DIR |= BIT0;        //conf LED at P1.0 for debugging
    P1OUT |= BIT0;    //LED P1.0 switched on at startup

    // Disable the GPIO power-on default high-impedance mode to activate
    // previously configured port settings
    PM5CTL0 &= ~LOCKLPM5;

    // Startup clock system with max DCO setting ~8MHz
    CSCTL0_H = CSKEY >> 8;              // Unlock clock registers
    CSCTL1 = DCOFSEL_3 | DCORSEL;         // Set DCO to 8MHz
    CSCTL2 = SELA__VLOCLK | SELS__DCOCLK | SELM__DCOCLK;
    CSCTL3 = DIVA__1 | DIVS__1 | DIVM__1; // Set all dividers
    CSCTL0_H = 0;                         // Lock CS registers


    // Configure USCI_A0 for UART mode

    UCA0CTLW0 = UCSWRST;          // enable software reset
    UCA0CTLW0 |= UCSSEL__SMCLK;   // CLK = SMCLK

    // Baud Rate calculation
    // 8000000/(16*9600) = 52.083
    // Fractional portion = 0.083
    // UCBRFx = int ( (52.083-52)*16) = 1
    UCA0BR0 = 52;                 // 8000000/16/9600
    UCA0BR1 = 0x00;
    UCA0MCTLW |= UCOS16 | UCBRF_1 | 0x4900;

    UCA0CTLW0 &= ~UCSWRST;        // Initialize eUSCI, disable software reset
    UCA0IE |= UCRXIE;             // Enable USCI_A0 RX interrupt

    UCA0TXBUF = 0x41;                 //test send 'A'

   __bis_SR_register(LPM0_bits | GIE);    // Enter LPM3, interrupts enabled
   __no_operation();









    int i;
    while(1)
    {
       measurement_loop(&GL_heart_rate_measurement_t);

       communication_loop();


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

   //sent_packet(&packet_to_be_sent_t);
}

void uart_init()
{
    /* Tx --> P6.0
     * RX --> P6.1
     */

    /* put UART A0 into software reset */
    UCA3CTLW0 |= UCSWRST;

    /* select SMCLK */
    UCA3BRW |= UCSSEL__SMCLK;

    /* prescaler */
    UCA3BRW = 8;

    /* configure modulation settings + low frequency */
    UCA3MCTLW |= 0xD600;

    /* setup TX and RX ports */
    P6SEL0 |= BIT0 | BIT1; // USCI_A0 UART operation
    P6SEL1 &= ~(BIT0 | BIT1);

    /* turn on I/O */
    PM5CTL0 &= ~LOCKLPM5;

    /* turn off reset */
    UCA3CTLW0 &= ~UCSWRST;
}








#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)

 #pragma vector=USCI_A0_VECTOR
 __interrupt void USCI_A0_ISR(void)

 #elif defined(__GNUC__)
 void __attribute__ ((interrupt(USCI_A0_VECTOR))) USCI_A0_ISR (void)
 #else
 #error Compiler not supported!
 #endif
 {
     switch(__even_in_range(UCA0IV, USCI_UART_UCTXCPTIFG))
     {
         case USCI_NONE:
             break;
         case USCI_UART_UCRXIFG:
             while(!(UCA0IFG & UCTXIFG));
             UCA0TXBUF = UCA0RXBUF;        //echo
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

