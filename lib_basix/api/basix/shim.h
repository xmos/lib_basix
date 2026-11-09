// Copyright 2026 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#ifndef BASIX_SHIM_H_
#define BASIX_SHIM_H_

#include <stdint.h>

#ifdef __XC__
#define SHIM_PORT_TYPE port
#else
#include <xcore/port.h>
#define SHIM_PORT_TYPE port_t
#endif

/**
 * \file shim.h
 * \brief Configure the shim control register.
 */

/** Operating modes encoded for the shim's MSB-first control transfer. */
typedef enum {
    /** Disable the shim. Pads are directly connected to the ports as normal */
    SHIM_MODE_DISABLED          = 0b011,
    /** Configure the shim for I3C. */
    SHIM_MODE_I3C               = 0b010,
    /** Configure the shim as a SoundWire master. */
    SHIM_MODE_SOUNDWIRE_MASTER  = 0b001,
    /** Configure the shim as a SoundWire slave. */
    SHIM_MODE_SOUNDWIRE_SLAVE   = 0b000,
} shim_mode_t;

/** Clock sources encoded for the shim's MSB-first control transfer. */
typedef enum {
    /** Use the system-on-chip clock from PLL0. This is typically the main 600 MHz or 800 MHz main core clock */
    SHIM_CLOCK_SOC              = 0b000,
    /** Use the DDR clock from the DDR controller. */
    SHIM_CLOCK_DDR              = 0b100
} shim_clock_t;


// PADCTRL bit positions for shift register. DO NOT EDIT
#define SHIM_NUM_CTRL_BITS              3   // Shared by all modes; increase when the remaining control bits are implemented.
#define SHIM_PADCTRL_CLK_BIT            20  // Drive strength
#define SHIM_PADCTRL_DATA_BIT           21  // Drive strength
#define SHIM_PADCTRL_LATCH_N_BIT        22  // Slew rate control


#if defined(__cplusplus) || defined(__XC__)
extern "C" {
#endif

/**
 * Configure the shim mode and clock, then apply the configuration.
 * \param mode The operating mode to select.
 * \param clock The clock source to select.
 * \param p_shim_config The port connected to the shim configuration interface.
 * 
 * @note p_shim_config must be enabled before calling this function.
 */
void shim_enable(const shim_mode_t mode, const shim_clock_t clock, const SHIM_PORT_TYPE p_shim_config);

/**
 * Disable the shim.
 * \param p_shim_config The port connected to the shim configuration interface.
 */
void shim_disable(const SHIM_PORT_TYPE p_shim_config);

/**
 * Shift the selected mode and clock configuration into the shim control register.
 * Call \ref shim_latch to apply the configuration. These split operations allow
 * synchronization with the clock.
 * \param mode The operating mode to select.
 * \param clock The clock source to select.
 * \param p_shim_config The port connected to the shim configuration interface.
 * 
 * @note p_shim_config must be enabled before calling this function.
 * @note shim_latch() must be called after shim_configure to apply the configuration.
 * @note Only one configuration may be pending; call shim_latch before another shim_configure.
 */
void shim_configure(const shim_mode_t mode, const shim_clock_t clock, const SHIM_PORT_TYPE p_shim_config);

/**
 * Apply the configuration previously shifted into the shim control register.
 * This operation typically executes just a couple of thread cycles after calling. 
 * \param p_shim_config The port connected to the shim configuration interface.
 */
void shim_latch(const SHIM_PORT_TYPE p_shim_config);

#undef SHIM_PORT_TYPE

#if defined(__cplusplus) || defined(__XC__)
}
#endif

#endif // BASIX_SHIM_H_
