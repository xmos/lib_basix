// Copyright 2026 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#include <xs1.h>
#include <xcore/port.h>
#include "basix/shim.h"

static uint32_t shim_ctrl_reg;

void shim_configure(const shim_mode_t mode, const shim_clock_t clock, const port_t p_shim_config)
{
    // Enum values are encoded for this shared three-bit MSB-first transfer.

    uint32_t shim_register_to_write = mode | clock;

    uint32_t ctrl_reg = (1 << SHIM_PADCTRL_LATCH_N_BIT) | XS1_SETC_MODE_SETPADCTRL; // Clock low, latch off
    port_write_control_word(p_shim_config, ctrl_reg);

    for (unsigned bit = 0; bit < SHIM_NUM_CTRL_BITS; ++bit) {
        const unsigned shift = SHIM_NUM_CTRL_BITS - 1 - bit;
        if ((shim_register_to_write >> shift) & 0x01) {
            ctrl_reg |= (1 << SHIM_PADCTRL_DATA_BIT);
        } else {
            ctrl_reg &= ~(1 << SHIM_PADCTRL_DATA_BIT);
        }
        port_write_control_word(p_shim_config, ctrl_reg); // set data bit

        ctrl_reg |= (1 << SHIM_PADCTRL_CLK_BIT); // rising clock edge
        port_write_control_word(p_shim_config, ctrl_reg);

        ctrl_reg &= ~(1 << SHIM_PADCTRL_CLK_BIT); // falling clock edge
        port_write_control_word(p_shim_config, ctrl_reg);
    }
    ctrl_reg &= ~(1 << SHIM_PADCTRL_LATCH_N_BIT); // Latch in data
    shim_ctrl_reg = ctrl_reg;
}

void shim_latch(const port_t p_shim_config)
{
    // This turns the shim on. It happens in the next processor cycle after the IN reading zero, so safe up to about 37.5MHz clock
    port_write_control_word(p_shim_config, shim_ctrl_reg);

    shim_ctrl_reg |= (1 << SHIM_PADCTRL_CLK_BIT); // Clock high while latch is active
    port_write_control_word(p_shim_config, shim_ctrl_reg);

    shim_ctrl_reg &= ~(1 << SHIM_PADCTRL_CLK_BIT); // Clock low
    shim_ctrl_reg |= (1 << SHIM_PADCTRL_LATCH_N_BIT); // Deassert latch
    port_write_control_word(p_shim_config, shim_ctrl_reg);

    port_write_control_word(p_shim_config, shim_ctrl_reg); // Dummy update
}


void shim_enable(const shim_mode_t mode, const shim_clock_t clock, const port_t p_shim_config)
{
    shim_configure(mode, clock, p_shim_config);
    shim_latch(p_shim_config);
}

void shim_disable(const port_t p_shim_config)
{
    shim_enable(SHIM_MODE_DISABLED, SHIM_CLOCK_SOC, p_shim_config);
}
