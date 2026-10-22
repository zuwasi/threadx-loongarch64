/***************************************************************************
 * Copyright (c) 2026 Engineering Software Lab Israel (ESL)
 * SPDX-License-Identifier: MIT
 **************************************************************************/

/*  External interrupt route for the UART receive interrupt (LA-REQ-105).

    Route, verified against the QEMU 8.2.2 sources hw/loongarch/virt.c,
    hw/intc/loongarch_pch_pic.c and hw/intc/loongarch_extioi.c:

        NS16550 UART IRQ  ->  PCH-PIC input 2
        PCH-PIC output    =   htmsi_vector[2]  (programmed here)
                          ->  EXTIOI input 2
        EXTIOI IPMAP group 0 -> IP0 -> CPU pin 2 -> ESTAT.IS bit 2

    The PCH-PIC is normal MMIO at 0x10000000.  The EXTIOI lives in IOCSR
    space at 0x1400 and is reached through the bsp_cpu.S accessors.  */

#include "bsp.h"
#include <stddef.h>

#define EXTIOI_IPMAP_GROUP0_IP0   0x00000001UL  /* byte 0: group 0 -> IP0 */
#define EXTIOI_COREMAP_CPU0       0x01010101UL  /* byte per IRQ: 1 << cpu */
#define PCH_PIC_UART_IRQ_MASK     (1UL << BSP_UART_PCH_IRQ)

static void (*ext_irq_notify)(void);

static volatile uint32_t *pch_pic_reg32(uint32_t offset)
{
    return (volatile uint32_t *) (BSP_PCH_PIC_BASE + (uintptr_t) offset); /* parasoft-suppress MISRAC2025-RULE_11_4-b "DEV-007" */
}

static volatile uint8_t *pch_pic_reg8(uint32_t offset)
{
    return (volatile uint8_t *) (BSP_PCH_PIC_BASE + (uintptr_t) offset); /* parasoft-suppress MISRAC2025-RULE_11_4-b "DEV-007" */
}

bool bsp_external_irq_init(void) /* parasoft-suppress MISRAC2025-RULE_8_7-a "DEV-011" */
{
    uint32_t extioi_base = (uint32_t) BSP_EXTIOI_IOCSR_BASE;
    uint32_t mask;
    uint32_t enable;
    bool     ok = true;

    /* PCH-PIC: send line 2 out on vector 2.  The vector table resets to
       zero, which would route every line to EXTIOI input 0.  */
    *pch_pic_reg8((uint32_t) BSP_PCH_PIC_REG_HTMSI_VEC + BSP_UART_PCH_IRQ) =
        (uint8_t) BSP_UART_PCH_IRQ;

    /* EXTIOI: input group 0 to IP0, UART input 2 to CPU 0.  */
    bsp_iocsr_write32(extioi_base + (uint32_t) BSP_EXTIOI_REG_IPMAP,
                      (uint32_t) EXTIOI_IPMAP_GROUP0_IP0);
    bsp_iocsr_write32(extioi_base + (uint32_t) BSP_EXTIOI_REG_COREMAP,
                      (uint32_t) EXTIOI_COREMAP_CPU0);

    /* EXTIOI: enable input 2 (read-modify-write its 32-input group).  */
    enable = bsp_iocsr_read32(extioi_base + (uint32_t) BSP_EXTIOI_REG_ENABLE);
    bsp_iocsr_write32(extioi_base + (uint32_t) BSP_EXTIOI_REG_ENABLE,
                      enable | (uint32_t) PCH_PIC_UART_IRQ_MASK);

    /* PCH-PIC: unmask line 2 (a set INT_MASK bit masks the line).  */
    mask = *pch_pic_reg32((uint32_t) BSP_PCH_PIC_REG_INT_MASK);
    *pch_pic_reg32((uint32_t) BSP_PCH_PIC_REG_INT_MASK) =
        mask & ~(uint32_t) PCH_PIC_UART_IRQ_MASK;

    /* Verify the route as the guest observes it.  */
    if (*pch_pic_reg8((uint32_t) BSP_PCH_PIC_REG_HTMSI_VEC + BSP_UART_PCH_IRQ) !=
        (uint8_t) BSP_UART_PCH_IRQ)
    {
        ok = false;
    }
    if ((*pch_pic_reg32((uint32_t) BSP_PCH_PIC_REG_INT_MASK) &
         (uint32_t) PCH_PIC_UART_IRQ_MASK) != 0U)
    {
        ok = false;
    }
    if ((bsp_iocsr_read32(extioi_base + (uint32_t) BSP_EXTIOI_REG_ENABLE) &
         (uint32_t) PCH_PIC_UART_IRQ_MASK) == 0U)
    {
        ok = false;
    }
    if (bsp_iocsr_read32(extioi_base + (uint32_t) BSP_EXTIOI_REG_IPMAP) !=
        (uint32_t) EXTIOI_IPMAP_GROUP0_IP0)
    {
        ok = false;
    }
    if (bsp_iocsr_read32(extioi_base + (uint32_t) BSP_EXTIOI_REG_COREMAP) !=
        (uint32_t) EXTIOI_COREMAP_CPU0)
    {
        ok = false;
    }

    return ok;
}

void bsp_external_irq_handler(void) /* parasoft-suppress MISRAC2025-RULE_8_7-a "DEV-011" */
{
    uint32_t received;

    /* End of interrupt at the EXTIOI first.  The UART line is level
       triggered but the EXTIOI latches, so clearing the service latch
       before draining guarantees that a byte arriving afterwards raises
       the CPU line again instead of being lost.  */
    bsp_iocsr_write32((uint32_t) BSP_EXTIOI_IOCSR_BASE + (uint32_t) BSP_EXTIOI_REG_COREISR,
                      (uint32_t) PCH_PIC_UART_IRQ_MASK);

    /* The exception entry already cleared CRMD.IE, so this runs with
       interrupts disabled (LA-REQ-009).  Drain the receive FIFO.  */
    received = bsp_uart_rx_isr();

    if ((received != 0U) && (ext_irq_notify != NULL))
    {
        (*ext_irq_notify)();
    }
}

void bsp_external_irq_notify_set(void (*notify)(void)) /* parasoft-suppress MISRAC2025-RULE_8_7-a "DEV-011" */
{
    ext_irq_notify = notify;
}
