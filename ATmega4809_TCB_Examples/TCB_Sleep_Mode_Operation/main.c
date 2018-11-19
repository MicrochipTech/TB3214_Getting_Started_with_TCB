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
#include <avr/sleep.h>

#define TCB_CMP_EXAMPLE_VALUE   (0x7fff)

void CLOCK_init (void);
void SLPCTRL_init (void);
void PORT_init (void);
void TCB0_init (void);

void CLOCK_init (void)
{
    /* Enable writing to protected register */
    CPU_CCP = CCP_IOREG_gc; 
    /* Disable CLK_PER Prescaler */
    CLKCTRL.MCLKCTRLB = 0 << CLKCTRL_PEN_bp; 
    
    /* Enable writing to protected register */
    CPU_CCP = CCP_IOREG_gc; 
    /* Select 32KHz Internal Ultra Low Power Oscillator (OSCULP32K) */
    CLKCTRL.MCLKCTRLA = CLKCTRL_CLKSEL_OSCULP32K_gc;
    
    /* Wait for system oscillator changing to finish */
    while (CLKCTRL.MCLKSTATUS & CLKCTRL_SOSC_bm)
    {
        ;
    }
}

void SLPCTRL_init (void)
{
    /* Enable sleep mode and select Standby mode */
    SLPCTRL.CTRLA = SLPCTRL_SMODE_gm | SLPCTRL_SMODE_STDBY_gc;
}

void PORT_init (void)
{
    PORTB.DIR |= PIN5_bm; /* Configure PB5 as digital output */
    PORTB.OUT |= PIN5_bm; /* Set initial level of PB5 */
}

void TCB0_init (void)
{
    /* Load the Compare or Capture register with the timeout value*/
    TCB0.CCMP = TCB_CMP_EXAMPLE_VALUE;
   
    /* Enable TCB and set CLK_PER divider to 1 (No Prescaling) */
    TCB0.CTRLA = TCB_CLKSEL_CLKDIV1_gc | TCB_ENABLE_bm | TCB_RUNSTDBY_bm;
    
    /* Enable Capture or Timeout interrupt */
    TCB0.INTCTRL = TCB_CAPT_bm;
}

ISR(TCB0_INT_vect)
{
    TCB0.INTFLAGS = TCB_CAPT_bm; /* Clear the interrupt flag */
    PORTB.IN = PIN5_bm; /* Toggle PB5 GPIO */
}

int main(void)
{
    CLOCK_init();
    SLPCTRL_init();
    PORT_init();
    TCB0_init();
    
    sei(); /* Enable Global Interrupts */
    
    while (1)
    {
        sleep_cpu(); /* Put CPU to sleep */
    }
}