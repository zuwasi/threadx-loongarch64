/***************************************************************************
 * Copyright (c) 2026 Engineering Software Lab Israel (ESL)
 * SPDX-License-Identifier: MIT
 **************************************************************************/

/*  string.h - freestanding memory functions for the ThreadX LoongArch64
    port.  The port is built with -nostdinc so no host or target C library
    header can be picked up.  These are the four functions GCC requires in a
    freestanding environment (memcpy, memmove, memset, memcmp).

    Requirement: LA-REQ-010  Freestanding runtime support
    Deviation:   DEV-001     MISRA C:2025 Rule 5.10 (reserved identifiers) */

#ifndef TX_LA64_STRING_H
#define TX_LA64_STRING_H

#include <stddef.h>

void *memcpy(void *dest, const void *src, size_t n);
void *memmove(void *dest, const void *src, size_t n);
void *memset(void *s, int c, size_t n);
int   memcmp(const void *s1, const void *s2, size_t n);

#endif /* TX_LA64_STRING_H */
