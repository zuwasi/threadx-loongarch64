/***************************************************************************
 * Copyright (c) 2026 Engineering Software Lab Israel (ESL)
 * SPDX-License-Identifier: MIT
 **************************************************************************/

/*  Minimal <stdio.h> for the upstream regression suite.
    Requirement: LA-REQ-104  */

#ifndef LA64_REGRESSION_STDIO_H
#define LA64_REGRESSION_STDIO_H

int  printf(const char *format, ...);
int  puts(const char *s);

#endif /* LA64_REGRESSION_STDIO_H */
