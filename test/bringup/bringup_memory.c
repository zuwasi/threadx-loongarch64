/***************************************************************************
 * Copyright (c) 2026 Engineering Software Lab Israel (ESL)
 * SPDX-License-Identifier: MIT
 **************************************************************************/

/* M6 unit tests for the freestanding C99 memory functions.

   Verifies: LA-REQ-010  */

#include <stdint.h>
#include <string.h>
#include "bsp.h"

static bool mem_check(bool condition, const char *name)
{
    bsp_uart_puts(condition ? "  PASS  " : "  FAIL  ");
    bsp_uart_puts(name);
    bsp_uart_puts("\n");
    return condition;
}

int main(void) /* parasoft-suppress MISRAC2025-RULE_8_6-a "separate freestanding images, one analysis project" */ /* parasoft-suppress MISRAC2025-RULE_5_8-b "separate freestanding images, one analysis project" */ /* parasoft-suppress MISRAC2025-RULE_8_3-c "separate freestanding images, one analysis project" */
{
    const uint8_t source[4] = { 0x00U, 0x7FU, 0x80U, 0xFFU };
    const uint8_t expected[4] = { 0x00U, 0x7FU, 0x80U, 0xFFU };
    uint8_t copied[4] = { 0U, 0U, 0U, 0U };
    uint8_t moved[6] = { 0x10U, 0x20U, 0x30U, 0x40U, 0x50U, 0x60U };
    uint8_t filled[4] = { 0U, 0U, 0U, 0U };
    const uint8_t less[3] = { 0x00U, 0x7FU, 0x7FU };
    bool ok = true;

    ok = mem_check((memcpy(copied, source, sizeof(source)) == copied) &&
               (memcmp(copied, expected, sizeof(expected)) == 0),
               "memcpy copies bytes and returns destination") && ok;
    ok = mem_check((memcpy(copied, source, 0U) == copied),
               "memcpy zero length") && ok;

    (void) memmove(&moved[1], &moved[0], 5U);
    ok = mem_check((moved[0] == 0x10U) && (moved[1] == 0x10U) &&
               (moved[2] == 0x20U) && (moved[5] == 0x50U),
               "memmove overlapping destination above source") && ok;
    (void) memmove(&moved[0], &moved[1], 5U);
    ok = mem_check((moved[0] == 0x10U) && (moved[1] == 0x20U) &&
               (moved[4] == 0x50U),
               "memmove overlapping destination below source") && ok;
    ok = mem_check((memmove(moved, moved, sizeof(moved)) == moved) &&
               (memmove(moved, moved, 0U) == moved),
               "memmove same and zero-length ranges") && ok;

    ok = mem_check((memset(filled, 0x1FF, sizeof(filled)) == filled) &&
               (filled[0] == 0xFFU) && (filled[3] == 0xFFU),
               "memset converts value to unsigned byte") && ok;
    ok = mem_check((memset(filled, 0, 0U) == filled) && (filled[0] == 0xFFU),
               "memset zero length") && ok;

    ok = mem_check((memcmp(source, expected, sizeof(source)) == 0),
               "memcmp equal ranges") && ok;
    ok = mem_check((memcmp(source, less, sizeof(less)) > 0),
               "memcmp compares bytes as unsigned") && ok;
    ok = mem_check((memcmp(source, expected, 0U) == 0),
               "memcmp zero length") && ok;

    bsp_test_report(ok);
    return 0;
}
