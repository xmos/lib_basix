// Copyright 2026 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#include <xs1.h>

#include <xcore/port.h>
#include "basix/shim.h"
#include "basix/print.h"

#ifndef SHIM_TEST_MODE
#error SHIM_TEST_MODE must be set by the build configuration
#endif

#ifndef SHIM_TEST_CLOCK
#error SHIM_TEST_CLOCK must be set by the build configuration
#endif

int main(void)
{
    const port_t shim_config_ports[] = {XS1_PORT_4C, XS1_PORT_4E};
    const shim_mode_t mode = SHIM_TEST_MODE;
    const shim_clock_t clock = SHIM_TEST_CLOCK;

    for (unsigned port_index = 0; port_index < sizeof(shim_config_ports) / sizeof(shim_config_ports[0]); ++port_index) {
        const port_t shim_config_port = shim_config_ports[port_index];
        port_enable(shim_config_port);

        shim_enable(mode, clock, shim_config_port);
        shim_configure(mode, clock, shim_config_port);
        shim_latch(shim_config_port);

        shim_disable(shim_config_port);
        port_disable(shim_config_port);
    }


    printstr("PASS\n");
    return 0;
}