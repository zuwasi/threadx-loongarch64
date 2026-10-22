#!/usr/bin/env python3
# Copyright (c) 2026 Engineering Software Lab Israel (ESL)
# SPDX-License-Identifier: MIT
"""Generate an uncommitted UART transport copy of Parasoft's runtime source.

Parasoft's license prohibits redistributing its runtime source. This helper
patches the copy in the caller's build directory, leaving the installation
and repository source untouched.
"""

from __future__ import annotations

import argparse
from pathlib import Path


PLACEHOLDER = '''#if CPPTEST_CUSTOM_COMMUNICATION

static int cpptestInitializeStream(void)
{
    /* Custom implementation */
    return 1;
}

static void cpptestFinalizeStream(void)
{
    /* Custom implementation */
}

static int cpptestSendData(const char* data, unsigned size)
{
    /* Custom implementation */
    return 1;
}

static int cpptestFlushData(void)
{
    /* Custom implementation */
    return 1;
}

#endif /* CPPTEST_CUSTOM_COMMUNICATION */'''

UART_TRANSPORT = '''#if CPPTEST_CUSTOM_COMMUNICATION

#include "board.h"

#define CPPTEST_UART_THR_OFFSET  0U
#define CPPTEST_UART_LSR_OFFSET  5U
#define CPPTEST_UART_LSR_THRE    0x20U

static void cpptestUartPutc(char value)
{
    volatile unsigned char* const uart_thr =
        (volatile unsigned char*) (BSP_UART0_BASE + (uintptr_t) CPPTEST_UART_THR_OFFSET);
    volatile unsigned char* const uart_lsr =
        (volatile unsigned char*) (BSP_UART0_BASE + (uintptr_t) CPPTEST_UART_LSR_OFFSET);

    while ((*uart_lsr & (unsigned char) CPPTEST_UART_LSR_THRE) == 0U)
    {
    }

    *uart_thr = (unsigned char) value;
}

static void cpptestUartWriteText(const char* text)
{
    while (*text != '\\0')
    {
        cpptestUartPutc(*text);
        text++;
    }
}

static void cpptestUartWriteHex(unsigned char value)
{
    static const char digits[16] =
    {
        '0', '1', '2', '3', '4', '5', '6', '7',
        '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'
    };

    cpptestUartPutc(digits[(value >> 4U) & 0x0FU]);
    cpptestUartPutc(digits[value & 0x0FU]);
}

static int cpptestInitializeStream(void)
{
    return 1;
}

static void cpptestFinalizeStream(void)
{
}

static int cpptestSendData(const char* data, unsigned size)
{
    unsigned index;
    unsigned shift;

    cpptestUartPutc('\\n');
    cpptestUartWriteText("@@CPPTCOV:");
    for (shift = 32U; shift > 0U; shift -= 4U)
    {
        static const char digits[16] =
        {
            '0', '1', '2', '3', '4', '5', '6', '7',
            '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'
        };

        cpptestUartPutc(digits[(size >> (shift - 4U)) & 0x0FU]);
    }
    cpptestUartPutc(':');

    for (index = 0U; index < size; index++)
    {
        cpptestUartWriteHex((unsigned char) data[index]);
    }

    cpptestUartPutc('\\r');
    cpptestUartPutc('\\n');
    return 1;
}

static int cpptestFlushData(void)
{
    return 1;
}

#endif /* CPPTEST_CUSTOM_COMMUNICATION */'''


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("source", type=Path, help="installed Parasoft runtime source")
    parser.add_argument("output", type=Path, help="generated build-directory copy")
    args = parser.parse_args()

    source = args.source.read_text(encoding="utf-8")
    if source.count(PLACEHOLDER) != 1:
        raise SystemExit("unexpected Parasoft runtime custom-communication template")

    args.output.parent.mkdir(parents=True, exist_ok=True)
    args.output.write_text(source.replace(PLACEHOLDER, UART_TRANSPORT), encoding="utf-8")
    print(f"Generated UART transport runtime copy: {args.output}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
