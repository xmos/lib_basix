// Copyright 2026 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#include <basix/architecture.h>

// basix_trap() must remain unconditional in release builds.
#ifdef NDEBUG
#undef NDEBUG
#endif
#include <xcore/assert.h>
#include <xcore/hwtimer.h>

void basix_trap(void)
{
    xassert(0);
    __builtin_unreachable();
}

uint32_t basix_time_now(void)
{
    return get_reference_time();
}

void basix_delay_ticks(uint32_t ticks)
{
    hwtimer_t timer = hwtimer_alloc();
    hwtimer_delay(timer, ticks);
    hwtimer_free(timer);
}

void basix_delay_microseconds(uint32_t microseconds)
{
    xassert(microseconds <= INT32_MAX / (BASIX_TIMER_HZ / 1000000));
    basix_delay_ticks(microseconds * (BASIX_TIMER_HZ / 1000000));
}

void basix_delay_milliseconds(uint32_t milliseconds)
{
    xassert(milliseconds <= INT32_MAX / (BASIX_TIMER_HZ / 1000));
    basix_delay_ticks(milliseconds * (BASIX_TIMER_HZ / 1000));
}

void basix_delay_seconds(uint32_t seconds)
{
    xassert(seconds <= INT32_MAX / BASIX_TIMER_HZ);
    basix_delay_ticks(seconds * BASIX_TIMER_HZ);
}
