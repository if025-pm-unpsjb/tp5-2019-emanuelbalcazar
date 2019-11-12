#include "mbed.h"       // Librería mbed
#include "FreeRTOS.h"   // Definiciones principales de FreeRTOS

void vConfigureTimerForRunTimeStats( void )
{
const unsigned long TCR_COUNT_RESET = 2,
                    CTCR_CTM_TIMER = 0x00,
                    TCR_COUNT_ENABLE = 0x01;

    /* Power up and feed the timer with a clock. */
    LPC_SC->PCONP |= 0x02UL;
    LPC_SC->PCLKSEL0 = (LPC_SC->PCLKSEL0 & (~(0x3<<2))) | (0x01 << 2);

    /* Reset Timer 0 */
    LPC_TIM1->TCR = TCR_COUNT_RESET;

    /* Just count up. */
    LPC_TIM1->CTCR = CTCR_CTM_TIMER;

    /* Prescale to a frequency that is good enough to get a decent resolution,
    but not too fast so as to overflow all the time. */
    LPC_TIM1->PR =  ( SystemCoreClock / 10000UL ) - 1UL;

    /* Start the counter. */
    LPC_TIM1->TCR = TCR_COUNT_ENABLE;
}
