/***************************************************************************
 * Copyright (c) 2026 Engineering Software Lab Israel (ESL)
 * SPDX-License-Identifier: MIT
 **************************************************************************/

/*  Minimal printf for the freestanding regression suite.  The build uses
    -nostdinc, so the suite's <stdio.h> comes from libc/include and all
    console output flows through the BSP UART (LA-REQ-104).

    Supported conversions: %s, %c, %d, %i, %u, %x, %X, a single 'l' length
    modifier, and %%.  Unsupported conversions are copied verbatim, the
    same behaviour as the upstream RISC-V harness printf.  */

#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>
#include "bsp.h"

static void put_counted(const char *text, unsigned int length)
{
    unsigned int index;

    for (index = 0U; index < length; index++)
    {
        bsp_uart_putc(text[index]);
    }
}

static void put_string(const char *text)
{
    const char *cursor = text;

    if (cursor != NULL)
    {
        while (*cursor != '\0')
        {
            bsp_uart_putc(*cursor);
            cursor++;
        }
    }
}

static void put_unsigned(unsigned long value, unsigned int base, bool upper)
{
    static const char digits_lower[16] =
    {
        '0', '1', '2', '3', '4', '5', '6', '7',
        '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'
    };
    static const char digits_upper[16] =
    {
        '0', '1', '2', '3', '4', '5', '6', '7',
        '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'
    };
    const char *digits = (upper ? digits_upper : digits_lower);
    char          buffer[20];  /* 2^64 - 1 needs 20 decimal digits.  */
    unsigned int  digit_count = 0U;
    unsigned long remaining   = value;

    do
    {
        buffer[digit_count] = digits[remaining % (unsigned long) base];
        digit_count++;
        remaining = remaining / (unsigned long) base;
    } while ((remaining != 0UL) &&
             (digit_count < (unsigned int) (sizeof(buffer) / sizeof(buffer[0]))));

    while (digit_count > 0U)
    {
        digit_count--;
        bsp_uart_putc(buffer[digit_count]);
    }
}

static void put_signed(long value)
{
    unsigned long magnitude;

    if (value < 0L)
    {
        bsp_uart_putc('-');
        /* Split the negation so LONG_MIN cannot overflow.  */
        magnitude = (unsigned long) (-(value + 1L)) + 1UL;
    }
    else
    {
        magnitude = (unsigned long) value;
    }

    put_unsigned(magnitude, 10U, false);
}

int printf(const char *format, ...)
{
    va_list arguments;
    const char *cursor = format;
    const char *segment = format;
    ptrdiff_t span;
    bool is_long;

    va_start(arguments, format);

    while (*cursor != '\0')
    {
        if (*cursor == '%')
        {
            /* Flush the literal segment before the conversion.  */
            if (cursor > segment)
            {
                span = cursor - segment;
                put_counted(segment, (unsigned int) span);
            }

            cursor++;
            is_long = false;
            if (*cursor == 'l')
            {
                is_long = true;
                cursor++;
            }

            switch (*cursor)
            {
                case 's':
                {
                    const char *text = va_arg(arguments, const char *);
                    put_string(text);
                    break;
                }

                case 'c':
                {
                    int ch = va_arg(arguments, int);
                    bsp_uart_putc((char) ch);
                    break;
                }

                case 'd':
                case 'i':
                {
                    if (is_long)
                    {
                        put_signed(va_arg(arguments, long));
                    }
                    else
                    {
                        put_signed((long) va_arg(arguments, int));
                    }
                    break;
                }

                case 'u':
                {
                    if (is_long)
                    {
                        put_unsigned(va_arg(arguments, unsigned long), 10U, false);
                    }
                    else
                    {
                        put_unsigned((unsigned long) va_arg(arguments, unsigned int),
                                     10U, false);
                    }
                    break;
                }

                case 'x':
                {
                    if (is_long)
                    {
                        put_unsigned(va_arg(arguments, unsigned long), 16U, false);
                    }
                    else
                    {
                        put_unsigned((unsigned long) va_arg(arguments, unsigned int),
                                     16U, false);
                    }
                    break;
                }

                case 'X':
                {
                    if (is_long)
                    {
                        put_unsigned(va_arg(arguments, unsigned long), 16U, true);
                    }
                    else
                    {
                        put_unsigned((unsigned long) va_arg(arguments, unsigned int),
                                     16U, true);
                    }
                    break;
                }

                case '%':
                {
                    bsp_uart_putc('%');
                    break;
                }

                case '\0':
                {
                    /* Format ends inside a conversion; stop here.  */
                    va_end(arguments);
                    return 0;
                }

                default:
                {
                    /* Unsupported conversion: copy it verbatim.  */
                    bsp_uart_putc('%');
                    if (is_long)
                    {
                        bsp_uart_putc('l');
                    }
                    bsp_uart_putc(*cursor);
                    break;
                }
            }

            cursor++;
            segment = cursor;
        }
        else
        {
            cursor++;
        }
    }

    if (cursor > segment)
    {
        span = cursor - segment;
        put_counted(segment, (unsigned int) span);
    }

    va_end(arguments);
    return 0;
}

int puts(const char *s)
{
    put_string(s);
    bsp_uart_putc('\n');
    return 0;
}
