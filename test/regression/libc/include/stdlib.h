/***************************************************************************
 * Copyright (c) 2026 Engineering Software Lab Israel (ESL)
 * SPDX-License-Identifier: MIT
 **************************************************************************/

/*  Minimal <stdlib.h> for the upstream regression suite.
    testcontrol.c calls external_exit() (EXTERNAL_EXIT), the kernel-setup
    test calls exit(), and threadx_thread_priority_change.c calls rand().
    All are implemented in libc/src/support.c.  */

#ifndef LA64_REGRESSION_STDLIB_H
#define LA64_REGRESSION_STDLIB_H

void  external_exit(unsigned int status) __attribute__((noreturn));
void  exit(int status) __attribute__((noreturn));
int   rand(void);
void  srand(unsigned int seed);

#endif /* LA64_REGRESSION_STDLIB_H */
