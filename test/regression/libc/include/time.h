/***************************************************************************
 * Copyright (c) 2026 Engineering Software Lab Israel (ESL)
 * SPDX-License-Identifier: MIT
 **************************************************************************/

/*  Minimal <time.h> for the upstream regression suite.
    The wall-clock tests use time() only for run-duration budgets;
    it is derived from the ThreadX tick counter (LA-REQ-102).  */

#ifndef LA64_REGRESSION_TIME_H
#define LA64_REGRESSION_TIME_H

typedef long  time_t;

time_t  time(time_t *timer);

#endif /* LA64_REGRESSION_TIME_H */
