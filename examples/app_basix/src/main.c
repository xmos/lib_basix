// Copyright 2026 XMOS LIMITED.
// This Software is subject to the terms of the XMOS Public Licence: Version 1.

#include <basix.h>

int main(void)
{
    printstr("lib_basix\n");
    basix_delay_ticks(1);
    basix_delay_milliseconds(1);
    return 0;
}
