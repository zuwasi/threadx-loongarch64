/***************************************************************************
 * Copyright (c) 2026 Engineering Software Lab Israel (ESL)
 * SPDX-License-Identifier: MIT
 **************************************************************************/

/*  Polled NS16550 console driver.

    Requirement: LA-REQ-104  */

#include "bsp.h"

#define UART_REG_THR        0U      /* Transmit holding (write)   */
#define UART_REG_IER        1U      /* Interrupt enable           */
#define UART_REG_FCR        2U      /* FIFO control (write)       */
#define UART_REG_LCR        3U      /* Line control               */
#define UART_REG_LSR        5U      /* Line status                */

#define UART_LCR_8N1        0x03U
#define UART_FCR_ENABLE     0x07U   /* Enable and clear both FIFOs */
#define UART_LSR_THRE       0x20U   /* Transmit holding empty      */

static volatile uint8_t *uart_reg(uint32_t offset)
{
    return (volatile uint8_t *) (BSP_UART0_BASE + (uintptr_t) offset); /* parasoft-suppress MISRAC2025-RULE_11_4-b "DEV-007" */
}

void bsp_uart_init(void) /* parasoft-suppress MISRAC2025-RULE_8_7-a "DEV-011" */
{
    *uart_reg(UART_REG_IER) = 0U;
    *uart_reg(UART_REG_LCR) = (uint8_t) UART_LCR_8N1;
    *uart_reg(UART_REG_FCR) = (uint8_t) UART_FCR_ENABLE;
}

void bsp_uart_putc(char c) /* parasoft-suppress MISRAC2025-RULE_8_7-a "DEV-011" */
{
    while ((*uart_reg(UART_REG_LSR) & (uint8_t) UART_LSR_THRE) == 0U)
    {
        /* Wait for room in the transmitter.  */
    }

    *uart_reg(UART_REG_THR) = (uint8_t) c;
}

void bsp_uart_puts(const char *s)
{
    const char *p = s;

    while (*p != '\0')
    {
        if (*p == '\n')
        {
            bsp_uart_putc('\r');
        }
        bsp_uart_putc(*p);
        p++;
    }
}

void bsp_uart_put_hex64(uint64_t value)
{
    static const char digits[16] =
    {
        '0', '1', '2', '3', '4', '5', '6', '7',
        '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'
    };
    uint32_t shift = 64U;

    bsp_uart_puts("0x");
    while (shift > 0U)
    {
        shift -= 4U;
        bsp_uart_putc(digits[(value >> shift) & 0xFU]);
    }
}

void bsp_uart_put_dec(uint32_t value) /* parasoft-suppress MISRAC2025-RULE_8_7-a "DEV-011" */
{
    static const char digits[10] =
    {
        '0', '1', '2', '3', '4', '5', '6', '7', '8', '9'
    };
    char     buffer[10];
    uint32_t digit_count = 0U;
    uint32_t rest        = value;

    do
    {
        uint32_t digit = rest % 10U;
        buffer[digit_count] = digits[digit];
        rest /= 10U;
        digit_count++;
    } while ((rest != 0U) && (digit_count < 10U));

    while (digit_count > 0U)
    {
        digit_count--;
        bsp_uart_putc(buffer[digit_count]);
    }
}
