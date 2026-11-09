# Copyright 2025-2026 XMOS LIMITED.
# This Software is subject to the terms of the XMOS Public Licence: Version 1.

import io
from pathlib import Path

import Pyxsim
import pytest
from Pyxsim import testers


@pytest.mark.parametrize(
    "config, mode, clock",
    [
        pytest.param("DISABLED_SOC", "DISABLED", "SOC_CLK", id="disabled-soc"),
        pytest.param("DISABLED_DDR", "DISABLED", "LPDDR_CLK", id="disabled-ddr"),
        pytest.param("I3C_SOC", "I3C", "SOC_CLK", id="i3c-soc"),
        pytest.param("I3C_DDR", "I3C", "LPDDR_CLK", id="i3c-ddr"),
        pytest.param("SOUNDWIRE_MASTER_SOC", "MASTER", "SOC_CLK", id="soundwire-master-soc"),
        pytest.param("SOUNDWIRE_MASTER_DDR", "MASTER", "LPDDR_CLK", id="soundwire-master-ddr"),
        pytest.param("SOUNDWIRE_SLAVE_SOC", "SLAVE", "SOC_CLK", id="soundwire-slave-soc"),
        pytest.param("SOUNDWIRE_SLAVE_DDR", "SLAVE", "LPDDR_CLK", id="soundwire-slave-ddr"),
    ],
)
def test_basix_shim(config, mode, clock, capfd):
    test_dir = Path(__file__).parent / "test_basix_shim"
    binary = test_dir / "bin" / config / f"test_basix_shim_{config}.xe"
    assert binary.is_file(), f"VX build did not produce {binary}"

    if mode == "DISABLED":
        expected_lines = ["PASS"]
    else:
        shim_name = "I3CShim" if mode == "I3C" else "SoundWireShim"
        expected_lines = [
            rf"{shim_name}\[0x8002\]\[0\]\[{port_index}\]: ENABLED with {clock} as {mode}"
            for port_index in (0, 0, 1, 1)
        ]
        expected_lines.append("PASS")
    expected_output = "\n".join(expected_lines) + "\n"
    tester = testers.ComparisonTester(io.StringIO(expected_output), regexp=True)
    simargs = ["--max-cycles", "150000", "--debug-periph"]


    result = Pyxsim.run_on_simulator_(
        binary,
        simargs=simargs,
        tester=tester,
        capfd=capfd,
    )

    if "I3C" in config:
        pytest.xfail("I3C shim simulation is not supported in xsim yet")

    if not result:
        captured = capfd.readouterr()
        diagnostics = "\n".join(
            part
            for part in ("\n".join(tester.failures), captured.out, captured.err)
            if part
        )
        pytest.fail(diagnostics or "xsim run failed without captured diagnostics")