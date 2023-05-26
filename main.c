/*!
    \file    main.c
    \brief   GPIO running led

    \version 2016-08-15, V1.0.0, demo for GD32F4xx
    \version 2018-12-12, V2.0.0, demo for GD32F4xx
*/

/*
    Copyright (c) 2018, GigaDevice Semiconductor Inc.

    Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

    1. Redistributions of source code must retain the above copyright notice, this
       list of conditions and the following disclaimer.
    2. Redistributions in binary form must reproduce the above copyright notice,
       this list of conditions and the following disclaimer in the documentation
       and/or other materials provided with the distribution.
    3. Neither the name of the copyright holder nor the names of its contributors
       may be used to endorse or promote products derived from this software without
       specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
OF SUCH DAMAGE.
*/
#include <stdio.h>
#include "gd32f4xx.h"
#include "systick.h"
#include "SEGGER_RTT.h"

/*!
    \brief      main function
    \param[in]  none
    \param[out] none
    \retval     none
*/
int main(void)
{
    /* configure systick */
    systick_config();

    rcu_periph_clock_enable(RCU_GPIOD);

    rcu_timer_clock_prescaler_config(RCU_TIMER_PSC_MUL4);
    gpio_mode_set(GPIOD, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO_PIN_4);

    timer_parameter_struct timer6initpara;
    rcu_periph_clock_enable(RCU_TIMER6);

    timer6initpara.clockdivision = TIMER_CKDIV_DIV1;
    timer6initpara.alignedmode = TIMER_COUNTER_EDGE;
    timer6initpara.counterdirection = TIMER_COUNTER_UP;
    timer6initpara.period = 10000 - 1;
    timer6initpara.prescaler = 20000 - 1;
    timer_init(TIMER6, &timer6initpara);
    timer_interrupt_enable(TIMER6, TIMER_INT_UP);
    nvic_irq_enable(TIMER6_IRQn, 1, 1);
    timer_enable(TIMER6);

    while (1)
    {
        // delay_1ms(1000);
        // SEGGER_RTT_printf(0, "Test\n");
    }
}

uint16_t seconds, minutes, hours;

void TIMER6_IRQHandler(void)
{
    if (SET == timer_interrupt_flag_get(TIMER6, TIMER_INT_UP))
    {
        seconds++;
        if (seconds == 60)
        {
            seconds = 0;
            minutes++;
        }
        if (minutes == 60)
        {
            minutes = 0;
            hours++;
        }
        if (hours == 24)
        {
            hours = 0;
        }
        SEGGER_RTT_printf(0, "One second elapsed %d:%d:%d\n", hours, minutes, seconds);
        gpio_bit_toggle(GPIOD, GPIO_PIN_4);
    }
    timer_interrupt_flag_clear(TIMER6, TIMER_INT_UP);
}