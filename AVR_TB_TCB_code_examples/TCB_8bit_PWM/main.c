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

void TCB3_init (void)
{
    /* Compare or Capture: 0x80FF */
    TCB3.CCMP = 0x80FF;

    /* Enable TCB3 and Divide CLK_PER by 2 */
    TCB3.CTRLA |= TCB_ENABLE_bm;
    TCB3.CTRLA |= TCB_CLKSEL_CLKDIV2_gc;
    
    /* Enable Pin Output and configure TCB in 8-bit PWM mode */
    TCB3.CTRLB |= TCB_CCMPEN_bm;
    TCB3.CTRLB |= TCB_CNTMODE_PWM8_gc;
}

int main(void)
{
    /* Write "IOREG" key to CPU.CPP */
    CPU_CCP = CCP_IOREG_gc;
    /* Enable Prescaler and set Prescaler Division to 64 */
    CLKCTRL.MCLKCTRLB = CLKCTRL_PDIV_64X_gc | CLKCTRL_PEN_bm;
    
    /* Write "IOREG" key to CPU.CPP */
    CPU_CCP = CCP_IOREG_gc;
    /* Select 32KHz Internal Ultra Low Power Oscillator (OSCULP32K) */
    CLKCTRL.MCLKCTRLA = CLKCTRL_CLKSEL_OSCULP32K_gc;
    
    /* wait for system oscillator changing to finish */
    while (CLKCTRL.MCLKSTATUS & CLKCTRL_SOSC_bm)
    {
        ;
    }    
    
    PORTB_DIR |= PIN5_bm;
    PORTB_OUT |= PIN5_bm;
    
    TCB3_init();
    
    while (1)
    {
        ;
    }
}