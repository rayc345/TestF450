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
#include "gd25qxx.h"

#define BUFFER_SIZE 256
#define TX_BUFFER_SIZE (countof(tx_buffer) - 1)
#define RX_BUFFER_SIZE 0xFF

#define countof(a) (sizeof(a) / sizeof(*(a)))

#define FLASH_WRITE_ADDRESS 0x000000
#define FLASH_READ_ADDRESS FLASH_WRITE_ADDRESS

uint8_t tx_buffer[256];
uint8_t rx_buffer[256];

ErrStatus memory_compare(uint8_t *src, uint8_t *dst, uint16_t length);

int main(void)
{
    /* configure systick */
    systick_config();

    spi_flash_init();

    uint32_t flash_id = spi_flash_read_id();
    printf("Flash ID %X\n", flash_id);

    printf("\n\rWrite to tx_buffer:\n");

    /* printf tx_buffer value */
    for (uint16_t i = 0; i < BUFFER_SIZE; i++)
    {
        tx_buffer[i] = i;
        printf("0x%X ", tx_buffer[i]);

        if (15 == i % 16)
        {
            printf("\n");
        }
        delay_1ms(1);
    }

    printf("\nRead from rx_buffer:\n");

    /* erase the specified flash sector */
    spi_flash_sector_erase(FLASH_WRITE_ADDRESS);

    /* write tx_buffer data to the flash */
    spi_flash_buffer_write(tx_buffer, FLASH_WRITE_ADDRESS, 256);

    delay_1ms(10);
    /* read a block of data from the flash to rx_buffer */
    spi_flash_buffer_read(rx_buffer, FLASH_READ_ADDRESS, 256);

    /* printf rx_buffer value */
    for (uint16_t i = 0; i < BUFFER_SIZE; i++)
    {
        printf("0x%X ", rx_buffer[i]);
        if (15 == i % 16)
        {
            printf("\n");
        }
        delay_1ms(1);
    }
    uint8_t is_successful = 0;
    if (ERROR == memory_compare(tx_buffer, rx_buffer, 256))
    {
        printf("\nErr:Data Read and Write aren't Matching.\n");
        is_successful = 1;
    }

    /* spi qspi flash test passed */
    if (0 == is_successful)
    {
        printf("\nSPI-GD25Q40 Test Passed!\n");
    }

    while (1)
    {
        // printf("Start to send\n");
        // const uint8_t uText[] = "TestText";
        // SendViaUART(uText, 9);
        // uint8_t uTextReceive[9];
        // ReceiveViaUART(uTextReceive, 9);
        // printf("Gpt %s\n", uTextReceive);
    }
}

ErrStatus memory_compare(uint8_t *src, uint8_t *dst, uint16_t length)
{
    while (length--)
    {
        if (*src++ != *dst++)
        {
            return ERROR;
        }
    }
    return SUCCESS;
}