/**
 * \file main.c
 *
 * \brief Main source file.
 *
 (c) 2018 Microchip Technology Inc. and its subsidiaries.

    Subject to your compliance with these terms, you may use this software and
    any derivatives exclusively with Microchip products. It is your responsibility
    to comply with third party license terms applicable to your use of third party
    software (including open source software) that may accompany Microchip software.

    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
    WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
    PARTICULAR PURPOSE.

    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
    BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
    FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
    ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
    THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
 *
 */

#include <avr/io.h>
#include <avr/interrupt.h>

void TCB0_init (void)
{
    TCB0.CCMP = 0x7fff; /* Load the Compare or Capture register with the timeout value*/
    
    /* Enable TCB and set CLK_PER divider to 1 (No Prescaling) */
    TCB0.CTRLA = TCB_CLKSEL_CLKDIV1_gc | TCB_ENABLE_bm;
    
    TCB0.CTRLB = TCB_CNTMODE_TIMEOUT_gc; /* Configure TCB in Periodic Timeout mode */
    
    TCB0.INTCTRL = TCB_CAPT_bm; /* Enable Capture or Timeout interrupt */
    
    TCB0.EVCTRL = TCB_CAPTEI_bm | TCB_EDGE_bm; /* Enable Event Input and Event Edge*/
}

void EVENT_SYSTEM_Init (void)
{
    EVSYS.CHANNEL0 = EVSYS_GENERATOR_PORT1_PIN2_gc; /* Port 1 Pin 2 */
    EVSYS.USERTCB0 = EVSYS_CHANNEL_CHANNEL0_gc; /* Connect user to event channel 0 */
}

ISR(TCB0_INT_vect)
{
    TCB0.INTFLAGS = TCB_CAPT_bm; /* Clear the interrupt flag */
    PORTB.IN = PIN5_bm; /* Toggle PB5 GPIO */
}

int main(void)
{
    CPU_CCP = CCP_IOREG_gc; /* Write "IOREG" key to CPU.CPP */
    CLKCTRL.MCLKCTRLB = 0 << CLKCTRL_PEN_bp; /* Disable CLK_PER Prescaler */
    
    CPU_CCP = CCP_IOREG_gc; /* Write "IOREG" key to CPU.CPP */
    /* Select 32KHz Internal Ultra Low Power Oscillator (OSCULP32K) */
    CLKCTRL.MCLKCTRLA = CLKCTRL_CLKSEL_OSCULP32K_gc;
   
    /* wait for system oscillator changing to finish */
    while (CLKCTRL.MCLKSTATUS & CLKCTRL_SOSC_bm)
    {
        ;
    }
    
    TCB0_init();
    EVENT_SYSTEM_Init();
    
    PORTB.DIR |= PIN5_bm; /* Configure PB5 as digital output */
    PORTB.OUT |= PIN5_bm; /* Set initial level of PB5 */
    PORTB.DIR &= ~(PIN2_bm); /* Configure PB2 as digital input */
    PORTB.PIN2CTRL = PORT_PULLUPEN_bm; /* Enable the internal pullup */
    
    sei(); /* Enable Global Interrupts */
    
    while (1);
}

