# Copyright 2025-2026 XMOS LIMITED.
# This Software is subject to the terms of the XMOS Public Licence: Version 1.
from pathlib import Path

import Pyxsim
import pytest
from Pyxsim import testers


@pytest.fixture
def run_xsim(capfd):
    """Run an already-built binary; setup failures must not count as test passes."""
    def run(test_name):
        binary = Path(__file__).parent / test_name / "bin" / f"{test_name}.xe"
        assert binary.is_file(), f"Build the tests before running pytest: missing {binary}"
        expect_file = Path(__file__).parent / test_name / "pass.expect"
        with expect_file.open() as golden:
            tester = testers.ComparisonTester(golden, regexp=False)
            # The trailing-underscore API skips Pyxsim's implicit rebuild from ./build.
            result = Pyxsim.run_on_simulator_(
                binary,
                tester=tester,
                capfd=capfd,
                simargs=["--max-cycles", "15000000"],
                timeout=30,
            )
        assert result, "".join(tester.failures) or f"Simulator test failed: {test_name}"

    return run


@pytest.fixture
def level(request):
    return request.config.getoption("--level")

def pytest_addoption(parser):

    parser.addoption(
        "--level",
        action="store",
        default="smoke",
        choices=["smoke", "default", "extended"],
        help="Test coverage level",
    )
