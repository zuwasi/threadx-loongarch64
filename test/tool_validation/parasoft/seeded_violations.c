/***************************************************************************
 * Copyright (c) 2026 Engineering Software Lab Israel (ESL)
 * SPDX-License-Identifier: MIT
 **************************************************************************/

/*  Seeded MISRA C:2025 violations for Parasoft C/C++test tool validation
    (docs/safety/tools.md, section 3.2).

    Each violation below is labelled with the guideline it seeds.
    tools/parasoft/validate.sh analyses this file with the project
    configuration; every seeded violation must be reported (see
    test/tool_validation/parasoft/README.md for the record).

    This file is never built into any product or test.  It exists only to
    be analysed.  Do not fix the violations.  */

#include <stdint.h>

/* Seeded: Rule 2.3 - unused type declaration.  */
typedef char seeded_unused_type_check[1];

/* Seeded: Rule 11.4 - integer to pointer conversion (MMIO style).  */
volatile uint8_t *seeded_mmio(void)
{
    return (volatile uint8_t *) 0x10000000UL;
}

/* Seeded: Rule 10.2 - addition of two essentially character operands.  */
char seeded_char_arith(char offset)
{
    return (char) ('a' + offset);
}

/* Seeded: Rule 10.8 - composite expression cast to a wider essential type.  */
uint64_t seeded_widen(uint32_t input)
{
    return (uint64_t) (input + 1U);
}

/* Seeded: Rule 18.4 - '+' applied to a pointer expression.  */
uint32_t seeded_pointer_walk(const uint32_t *items)
{
    return *(items + 1);
}

/* Seeded: Rule 8.13 - pointer parameter that is only read.  */
uint32_t seeded_read_only(uint32_t *value)
{
    return *value;
}

/* Seeded: Rule 8.4 - external definition without a preceding declaration.  */
void seeded_no_prototype(void)
{
}

/* Seeded: Rule 17.7 - value returned by a non-void function is not used.  */
void seeded_discard(void)
{
    seeded_widen(0U);
}
