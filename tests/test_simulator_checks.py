# Copyright 2026 XMOS LIMITED.
# This Software is subject to the terms of the XMOS Public Licence: Version 1.

from pathlib import Path

import Pyxsim
import pytest


@pytest.mark.parametrize(
    "test_name", ["test_basix_pass", "test_basix_fail", "test_basix_xc"],
)
def test_false_simulator_result_fails(run_xsim, monkeypatch, test_name):
    monkeypatch.setattr(Path, "is_file", lambda path: True)
    monkeypatch.setattr(Pyxsim, "run_on_simulator_", lambda *args, **kwargs: False)
    with pytest.raises(AssertionError, match="Simulator test failed"):
        run_xsim(test_name)


@pytest.mark.parametrize(
    "test_name, output",
    [
        ("test_basix_pass", ""),
        ("test_basix_pass", "Simulator setup failed\n"),
        ("test_basix_fail", "PASS\n"),
        ("test_basix_fail", ""),
        ("test_basix_fail", "Unhandled exception: ILLEGAL_INSTRUCTION\n"),
        ("test_basix_xc", "PASS\n"),
    ],
)
def test_unexpected_output_fails(run_xsim, monkeypatch, test_name, output):
    monkeypatch.setattr(Path, "is_file", lambda path: True)
    monkeypatch.setattr(
        Pyxsim, "run_with_pyxsim", lambda *args, **kwargs: print(output, end=""),
    )
    with pytest.raises(AssertionError):
        run_xsim(test_name)


def test_prebuilt_runner_does_not_rebuild(run_xsim, monkeypatch):
    monkeypatch.setattr(Path, "is_file", lambda path: True)

    def unexpected_build(*args, **kwargs):
        pytest.fail("The simulator runner must not rebuild prebuilt firmware")

    def simulate(binary, simthreads, **kwargs):
        assert kwargs["simargs"] == ["--max-cycles", "15000000"]
        assert kwargs["timeout"] == 30
        print("PASS")

    monkeypatch.setattr(Pyxsim, "_build", unexpected_build)
    monkeypatch.setattr(Pyxsim, "run_with_pyxsim", simulate)
    run_xsim("test_basix_pass")


def test_missing_binary_fails(run_xsim, monkeypatch):
    monkeypatch.setattr(Path, "is_file", lambda path: False)
    with pytest.raises(AssertionError, match="Build the tests before running pytest"):
        run_xsim("test_basix_pass")
