// Copyright 2025-2026 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#include <basix.h>

int main()
{
    uint32_t before = basix_time_now();
    basix_delay_ticks(10);
    uint32_t elapsed = basix_time_now() - before;

    if (elapsed < 10)
        basix_trap();

#if defined(VX_TIMER_HZ)
    if (BASIX_TIMER_HZ != VX_TIMER_HZ)
        basix_trap();
#elif defined(XS1_TIMER_HZ)
    if (BASIX_TIMER_HZ != XS1_TIMER_HZ)
        basix_trap();
#else
    /* No toolchain macro available; BASIX_TIMER_HZ may be application-defined. */
#endif

    printstr("PASS\n");
    return 0;
}
