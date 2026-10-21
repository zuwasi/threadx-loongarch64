/***************************************************************************
 * Copyright (c) 2026 Engineering Software Lab Israel (ESL)
 * SPDX-License-Identifier: MIT
 **************************************************************************/

/*  System tick from the LoongArch constant timer.

    The timer counts at CC_FREQ * CC_MUL / CC_DIV (CPUCFG words 4 and 5).
    It runs in periodic mode and raises interrupt line TI (11).

    Requirements: LA-REQ-102, LA-REQ-103  */

#include "bsp.h"

#define CPUCFG_CC_FREQ          4U
#define CPUCFG_CC_MULDIV        5U

#define TCFG_EN                 0x1ULL
#define TCFG_PERIODIC           0x2ULL
#define TCFG_INITVAL_ALIGN      0x3ULL  /* InitVal low two bits are reserved */

static uint64_t timer_frequency_hz = 0U;

bool bsp_timer_init(uint32_t ticks_per_second) /* parasoft-suppress MISRAC2025-RULE_8_7-a "DEV-011" */
{
    bool     result = false;
    uint64_t cc_freq;
    uint32_t muldiv;
    uint32_t mul_value;
    uint32_t div_value;
    uint64_t cc_mul;
    uint64_t cc_div;
    uint64_t init_value;

    cc_freq = (uint64_t) bsp_cpucfg_read(CPUCFG_CC_FREQ);
    muldiv  = bsp_cpucfg_read(CPUCFG_CC_MULDIV);
    mul_value = muldiv & 0xFFFFU;
    div_value = (muldiv >> 16U) & 0xFFFFU;
    cc_mul  = (uint64_t) mul_value;
    cc_div  = (uint64_t) div_value;

    if ((ticks_per_second != 0U) && (cc_freq != 0U) && (cc_mul != 0U) && (cc_div != 0U))
    {
        timer_frequency_hz = (cc_freq * cc_mul) / cc_div;
        init_value = (timer_frequency_hz / (uint64_t) ticks_per_second) & ~TCFG_INITVAL_ALIGN;

        if (init_value != 0U)
        {
            bsp_timer_ack();
            bsp_timer_config_write(init_value | TCFG_PERIODIC | TCFG_EN);
            result = true;
        }
    }

    return result;
}

uint64_t bsp_timer_frequency(void) /* parasoft-suppress MISRAC2025-RULE_8_7-a "DEV-011" */
{
    return timer_frequency_hz;
}
