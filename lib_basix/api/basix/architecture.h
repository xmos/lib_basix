// Copyright 2026 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#ifndef BASIX_ARCHITECTURE_H_
#define BASIX_ARCHITECTURE_H_

#include <stdint.h>

/** Reference timer frequency. An override must match the hardware configuration. */
#ifndef BASIX_TIMER_HZ
#include <timer.h>
#if defined(VX_TIMER_HZ)
#define BASIX_TIMER_HZ VX_TIMER_HZ
#elif defined(XS1_TIMER_HZ)
#define BASIX_TIMER_HZ XS1_TIMER_HZ
#else
#error "No toolchain timer frequency definition available"
#endif
#endif

#if defined(__cplusplus) || defined(__XC__)
extern "C" {
#endif

/** Raise the architecture's assertion exception. */
#ifdef __XC__
void basix_trap(void);
#else
void basix_trap(void) __attribute__((noreturn));
#endif

/** Return the current reference-clock timestamp. */
uint32_t basix_time_now(void);

/** Delay for the specified number of reference-clock ticks. */
void basix_delay_ticks(uint32_t ticks);

#if defined(__cplusplus) || defined(__XC__)
}
#endif

#endif // BASIX_ARCHITECTURE_H_
