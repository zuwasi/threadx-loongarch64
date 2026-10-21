/***************************************************************************
 * Copyright (c) 2026 Engineering Software Lab Israel (ESL)
 * SPDX-License-Identifier: MIT
 **************************************************************************/

/*  Freestanding memory functions for the ThreadX LoongArch64 port.

    Byte-wise implementations, chosen for simplicity of verification over
    speed.  This file must be compiled with
    -fno-tree-loop-distribute-patterns so that GCC does not turn the loops
    back into calls to the functions being defined.

    Requirement: LA-REQ-010  Freestanding runtime support
    Deviation:   DEV-001     MISRA C:2025 Rule 5.10 (reserved identifiers) */

#include <stdint.h>
#include <string.h>

void *memcpy(void *dest, const void *src, size_t n) /* parasoft-suppress MISRAC2025-RULE_5_10-b "DEV-001" */ /* parasoft-suppress MISRAC2025-RULE_5_10-c "DEV-001" */
{
    uint8_t       *d = (uint8_t *) dest; /* parasoft-suppress MISRAC2025-RULE_11_5-b "DEV-009" */
    const uint8_t *s = (const uint8_t *) src; /* parasoft-suppress MISRAC2025-RULE_11_5-b "DEV-009" */
    size_t         i;

    for (i = 0U; i < n; i++)
    {
        d[i] = s[i];
    }

    return dest;
}

void *memmove(void *dest, const void *src, size_t n) /* parasoft-suppress MISRAC2025-RULE_5_10-b "DEV-001" */ /* parasoft-suppress MISRAC2025-RULE_5_10-c "DEV-001" */
{
    uint8_t       *d = (uint8_t *) dest; /* parasoft-suppress MISRAC2025-RULE_11_5-b "DEV-009" */
    const uint8_t *s = (const uint8_t *) src; /* parasoft-suppress MISRAC2025-RULE_11_5-b "DEV-009" */
    size_t         i;

    if ((uintptr_t) d <= (uintptr_t) s)
    {
        for (i = 0U; i < n; i++)
        {
            d[i] = s[i];
        }
    }
    else
    {
        for (i = n; i > 0U; i--)
        {
            d[i - 1U] = s[i - 1U];
        }
    }

    return dest;
}

void *memset(void *s, int c, size_t n) /* parasoft-suppress MISRAC2025-RULE_5_10-b "DEV-001" */ /* parasoft-suppress MISRAC2025-RULE_5_10-c "DEV-001" */ /* parasoft-suppress MISRAC2025-DIR_4_6-b "DEV-008" */
{
    uint8_t *d     = (uint8_t *) s; /* parasoft-suppress MISRAC2025-RULE_11_5-b "DEV-009" */
    uint8_t  value = (uint8_t) ((uint32_t) c & 0xFFU);
    size_t   i;

    for (i = 0U; i < n; i++)
    {
        d[i] = value; /* parasoft-suppress MISRAC2025-DIR_4_1-b "DEV-014" */
    }

    return s;
}

int memcmp(const void *s1, const void *s2, size_t n) /* parasoft-suppress MISRAC2025-RULE_5_10-b "DEV-001" */ /* parasoft-suppress MISRAC2025-RULE_5_10-c "DEV-001" */ /* parasoft-suppress MISRAC2025-DIR_4_6-b "DEV-008" */
{
    const uint8_t *a      = (const uint8_t *) s1; /* parasoft-suppress MISRAC2025-RULE_11_5-b "DEV-009" */
    const uint8_t *b      = (const uint8_t *) s2; /* parasoft-suppress MISRAC2025-RULE_11_5-b "DEV-009" */
    int            result = 0; /* parasoft-suppress MISRAC2025-DIR_4_6-b "DEV-008" */
    size_t         i      = 0U;

    while ((i < n) && (result == 0))
    {
        if (a[i] != b[i])
        {
            result = ((int) a[i]) - ((int) b[i]); /* parasoft-suppress MISRAC2025-DIR_4_6-b "DEV-008" */
        }
        i++;
    }

    return result;
}
