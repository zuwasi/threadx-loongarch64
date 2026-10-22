/***************************************************************************
 * Copyright (c) 2026 Engineering Software Lab Israel (ESL)
 * SPDX-License-Identifier: MIT
 **************************************************************************/

/*  NS16550 console driver: polled transmit, interrupt-driven receive.
    The receive FIFO is drained into a ring by the external interrupt
    handler (intr.c) over the LA-REQ-105 route.

    Requirements: LA-REQ-104, LA-REQ-105  */

#include "bsp.h"

#define UART_REG_THR        0U      /* Transmit holding (write)   */
#define UART_REG_RBR        0U      /* Receive buffer (read)      */
#define UART_REG_IER        1U      /* Interrupt enable           */
#define UART_REG_FCR        2U      /* FIFO control (write)        */
#define UART_REG_LCR        3U      /* Line control               */
#define UART_REG_LSR        5U      /* Line status                */

#define UART_LCR_8N1        0x03U
#define UART_FCR_ENABLE     0x07U   /* Enable and clear both FIFOs */
#define UART_LSR_THRE       0x20U   /* Transmit holding empty      */
#define UART_LSR_DR         0x01U   /* Receive data ready          */
#define UART_LSR_ERROR      0x1EU   /* OE, PE, FE and break        */
#define UART_IER_RDI        0x01U   /* Received-data interrupt    */

#define UART_RX_RING_SIZE   128U    /* power of two               */
#define UART_RX_RING_MASK   (UART_RX_RING_SIZE - 1U)

static volatile uint8_t *uart_reg(uint32_t offset)
{
    return (volatile uint8_t *) (BSP_UART0_BASE + (uintptr_t) offset); /* parasoft-suppress MISRAC2025-RULE_11_4-b "DEV-007" */
}

/* Receive ring: one producer (the external interrupt handler) and one
   consumer (a thread).  Head and tail are free-running counters.  */
static volatile uint8_t  uart_rx_ring[UART_RX_RING_SIZE];
static volatile uint32_t uart_rx_head;
static volatile uint32_t uart_rx_tail;
static volatile uint32_t uart_rx_isr_bytes;
static volatile uint32_t uart_rx_isr_calls;
static volatile uint32_t uart_rx_max_drain;
static volatile uint32_t uart_rx_overflow;
static volatile uint32_t uart_rx_line_errors;
static volatile uint64_t uart_rx_isr_crmd;

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

void bsp_uart_rx_irq_enable(void) /* parasoft-suppress MISRAC2025-RULE_8_7-a "DEV-011" */
{
    *uart_reg(UART_REG_IER) = (uint8_t) UART_IER_RDI;
}

void bsp_uart_rx_irq_disable(void) /* parasoft-suppress MISRAC2025-RULE_8_7-a "DEV-011" */
{
    *uart_reg(UART_REG_IER) = 0U;
}

uint32_t bsp_uart_rx_isr(void) /* parasoft-suppress MISRAC2025-RULE_8_7-a "DEV-011" */
{
    uint32_t head = uart_rx_head;
    uint32_t tail = uart_rx_tail;
    uint32_t taken = 0U;
    uint8_t  lsr;

    uart_rx_isr_calls = uart_rx_isr_calls + 1U;
    uart_rx_isr_crmd = bsp_crmd_read();

    lsr = *uart_reg(UART_REG_LSR);
    while ((lsr & (uint8_t) UART_LSR_DR) != 0U)
    {
        uint8_t data;

        if ((lsr & (uint8_t) UART_LSR_ERROR) != 0U)
        {
            uart_rx_line_errors = uart_rx_line_errors + 1U;
        }

        data = *uart_reg(UART_REG_RBR);

        if ((uint32_t) (head - tail) < UART_RX_RING_SIZE)
        {
            uart_rx_ring[head & UART_RX_RING_MASK] = data;
            head = head + 1U;
            tail = uart_rx_tail;
            taken = taken + 1U;
        }
        else
        {
            uart_rx_overflow = uart_rx_overflow + 1U;
        }

        lsr = *uart_reg(UART_REG_LSR);
    }

    uart_rx_head = head;
    uart_rx_isr_bytes = uart_rx_isr_bytes + taken;
    if (taken > uart_rx_max_drain)
    {
        uart_rx_max_drain = taken;
    }

    return taken;
}

uint32_t bsp_uart_rx_available(void) /* parasoft-suppress MISRAC2025-RULE_8_7-a "DEV-011" */
{
    uint32_t head = uart_rx_head;
    uint32_t tail = uart_rx_tail;

    return (uint32_t) (head - tail);
}

uint32_t bsp_uart_rx_read(uint8_t *buffer, uint32_t length) /* parasoft-suppress MISRAC2025-RULE_8_7-a "DEV-011" */
{
    uint32_t tail = uart_rx_tail;
    uint32_t copied = 0U;

    while (copied < length)
    {
        if (tail == uart_rx_head)
        {
            break;
        }
        buffer[copied] = uart_rx_ring[tail & UART_RX_RING_MASK];
        tail = tail + 1U;
        copied = copied + 1U;
    }

    uart_rx_tail = tail;
    return copied;
}

void bsp_uart_rx_stats_get(bsp_uart_rx_stats_t *stats) /* parasoft-suppress MISRAC2025-RULE_8_7-a "DEV-011" */
{
    stats->isr_bytes = uart_rx_isr_bytes;
    stats->isr_calls = uart_rx_isr_calls;
    stats->max_drain = uart_rx_max_drain;
    stats->overflow = uart_rx_overflow;
    stats->line_errors = uart_rx_line_errors;
    stats->isr_crmd = uart_rx_isr_crmd;
}
