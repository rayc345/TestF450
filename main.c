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
#include <stdbool.h>
#include "gd32f4xx.h"
#include "systick.h"

uint32_t adc_value[2];

uint16_t ADC_ReadValue(void)
{
    uint16_t Value = 0;
    adc_software_trigger_enable(ADC2, ADC_ROUTINE_CHANNEL); // 软件触发 ADC 转换开始

    if (SET == adc_flag_get(ADC2, ADC_FLAG_STRC))
    {
        delay_1ms(100);
        if (SET == adc_flag_get(ADC2, ADC_FLAG_EOC))
        {
            Value = adc_routine_data_read(ADC2);
        }
        adc_flag_clear(ADC2, ADC_FLAG_STRC | ADC_FLAG_EOC);
    }
    return Value;
}

int main(void)
{
    systick_config();

    rcu_periph_clock_enable(RCU_GPIOB);
    rcu_periph_clock_enable(RCU_GPIOF);
    rcu_periph_clock_enable(RCU_ADC2);
    rcu_periph_clock_enable(RCU_DMA1);
    rcu_periph_clock_enable(RCU_TIMER1);
    rcu_timer_clock_prescaler_config(RCU_TIMER_PSC_MUL4);
    adc_clock_config(ADC_ADCCK_PCLK2_DIV4);

    gpio_mode_set(GPIOF, GPIO_MODE_ANALOG, GPIO_PUPD_NONE, GPIO_PIN_5 | GPIO_PIN_6);

    gpio_mode_set(GPIOB, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_3);
    gpio_output_options_set(GPIOB, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_3);
    gpio_af_set(GPIOB, GPIO_AF_1, GPIO_PIN_3);

    // dma_single_data_parameter_struct dma_single_data_parameter;
    // dma_deinit(DMA1, DMA_CH0);
    // dma_single_data_parameter.periph_addr = (uint32_t)(&ADC_RDATA(ADC2));
    // dma_single_data_parameter.periph_inc = DMA_PERIPH_INCREASE_DISABLE;
    // dma_single_data_parameter.memory0_addr = (uint32_t)(adc_value);
    // dma_single_data_parameter.memory_inc = DMA_MEMORY_INCREASE_ENABLE;
    // dma_single_data_parameter.periph_memory_width = DMA_PERIPH_WIDTH_16BIT;
    // dma_single_data_parameter.circular_mode = DMA_CIRCULAR_MODE_ENABLE;
    // dma_single_data_parameter.direction = DMA_PERIPH_TO_MEMORY;
    // dma_single_data_parameter.number = 2;
    // dma_single_data_parameter.priority = DMA_PRIORITY_HIGH;
    // dma_single_data_mode_init(DMA1, DMA_CH0, &dma_single_data_parameter);

    // dma_channel_subperipheral_select(DMA1, DMA_CH0, DMA_SUBPERI2);
    // dma_circulation_enable(DMA1, DMA_CH0);
    // dma_channel_enable(DMA1, DMA_CH0);

    adc_deinit();
    adc_sync_mode_config(ADC_SYNC_MODE_INDEPENDENT);

    adc_special_function_config(ADC2, ADC_CONTINUOUS_MODE, DISABLE); // 配置 ADC 为连续模式（扫描模式只在 DMA 模式下使用）
    adc_special_function_config(ADC2, ADC_SCAN_MODE, DISABLE);       // 配置 ADC 为连续模式（扫描模式只在 DMA 模式下使用）

    adc_data_alignment_config(ADC2, ADC_DATAALIGN_RIGHT); // ADC 对齐设置为右对齐

    adc_channel_length_config(ADC2, ADC_ROUTINE_CHANNEL, 1); // 配置通道数

    adc_routine_channel_config(ADC2, 0, ADC_CHANNEL_4, ADC_SAMPLETIME_15); // 配置使用哪一个 ADC 通道

    adc_external_trigger_source_config(ADC2, ADC_ROUTINE_CHANNEL, ADC_EXTTRIG_ROUTINE_T0_CH0);
    adc_external_trigger_config(ADC2, ADC_ROUTINE_CHANNEL, EXTERNAL_TRIGGER_DISABLE);

    adc_enable(ADC2);
    adc_calibration_enable(ADC2); // 校准和复位校准 ADC

    // adc_special_function_config(ADC2, ADC_CONTINUOUS_MODE, ENABLE);
    // adc_special_function_config(ADC2, ADC_SCAN_MODE, ENABLE);
    // adc_data_alignment_config(ADC2, ADC_DATAALIGN_RIGHT);
    // adc_channel_length_config(ADC2, ADC_ROUTINE_CHANNEL, 2);
    // adc_routine_channel_config(ADC2, 0, ADC_CHANNEL_15, ADC_SAMPLETIME_144);
    // adc_routine_channel_config(ADC2, 1, ADC_CHANNEL_4, ADC_SAMPLETIME_144);

    // // adc_external_trigger_config(ADC2, ADC_ROUTINE_CHANNEL, EXTERNAL_TRIGGER_RISING);
    // // adc_external_trigger_source_config(ADC2, ADC_ROUTINE_CHANNEL, ADC_EXTTRIG_ROUTINE_T1_CH1);

    // adc_external_trigger_source_config(ADC2, ADC_ROUTINE_CHANNEL, ADC_EXTTRIG_REGULAR_T1_CH1);
    // adc_external_trigger_config(ADC2, ADC_ROUTINE_CHANNEL, EXTERNAL_TRIGGER_DISABLE);

    // /* ADC DMA function enable */
    // adc_dma_request_after_last_enable(ADC2);
    // adc_dma_mode_enable(ADC2);

    // adc_enable(ADC2);
    // delay_1ms(1);
    // adc_calibration_enable(ADC2);

    // /* enable ADC software trigger */
    // adc_software_trigger_enable(ADC2, ADC_ROUTINE_CHANNEL);

    // timer_oc_parameter_struct timer_ocintpara;
    // timer_parameter_struct timer_initpara;

    // timer_initpara.prescaler = 19999;
    // timer_initpara.alignedmode = TIMER_COUNTER_EDGE;
    // timer_initpara.counterdirection = TIMER_COUNTER_UP;
    // timer_initpara.period = 9999;
    // timer_initpara.clockdivision = TIMER_CKDIV_DIV1;
    // timer_initpara.repetitioncounter = 0;
    // timer_init(TIMER1, &timer_initpara);

    // timer_ocintpara.ocpolarity = TIMER_OC_POLARITY_HIGH;
    // timer_ocintpara.ocnpolarity = TIMER_OCN_POLARITY_HIGH;
    // timer_ocintpara.outputnstate = TIMER_CCXN_DISABLE;
    // timer_ocintpara.outputstate = TIMER_CCX_ENABLE;
    // timer_ocintpara.ocidlestate = TIMER_OC_IDLE_STATE_LOW;
    // timer_ocintpara.ocnidlestate = TIMER_OCN_IDLE_STATE_LOW;
    // timer_channel_output_config(TIMER1, TIMER_CH_1, &timer_ocintpara);

    // timer_channel_output_pulse_value_config(TIMER1, TIMER_CH_1, 3999);
    // timer_channel_output_mode_config(TIMER1, TIMER_CH_1, TIMER_OC_MODE_PWM0);
    // timer_channel_output_shadow_config(TIMER1, TIMER_CH_1, TIMER_OC_SHADOW_DISABLE);

    // timer_primary_output_config(TIMER1, ENABLE);
    // timer_auto_reload_shadow_enable(TIMER1);
    // // timer_enable(TIMER1);

    while (1)
    {
        // while (!dma_flag_get(DMA1, DMA_CH0, DMA_FLAG_FTF))
        // {
        // };

        // /* clear channel1 transfer complete flag */
        // dma_flag_clear(DMA1, DMA_CH0, DMA_FLAG_FTF);

        // printf(" the data adc_value[0] is %X \r\n", adc_value[0]);
        // printf(" the data adc_value[1] is %X \r\n", adc_value[1]);
        // printf("\r\n");

        delay_1ms(500);
        uint16_t ADC_Value = ADC_ReadValue();
        printf("ADC Channel Value = %d\r\n ", ADC_Value);
    }
}
