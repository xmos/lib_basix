# Copyright 2025-2026 XMOS LIMITED.
# This Software is subject to the terms of the XMOS Public Licence: Version 1.

def test_basix_fail(run_xsim):
    # Match the expected ECALL output, not an arbitrary simulator/setup failure.
    run_xsim("test_basix_fail")
