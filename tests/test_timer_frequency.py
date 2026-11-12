# Copyright 2026 XMOS LIMITED.
# This Software is subject to the terms of the XMOS Public Licence: Version 1.

import subprocess
from pathlib import Path

import pytest


XS_HEADER = "#define XS1_TIMER_HZ 100000000U\n"
VX_HEADER = "#define VX_TIMER_HZ 125000000U\n"
BOTH_HEADERS = XS_HEADER + VX_HEADER
OVERRIDE_HEADER = '#error "An application override must not require timer.h"\n'


@pytest.mark.parametrize("language", ["c", "c++"])
@pytest.mark.parametrize(
    "timer_header, override, expected",
    [
        (XS_HEADER, None, 100000000),
        (VX_HEADER, None, 125000000),
        (BOTH_HEADERS, None, 125000000),
        (OVERRIDE_HEADER, 80000000, 80000000),
        ("", None, None),
    ],
    ids=["xs", "vx", "vx-precedence", "application-override", "missing-frequency"],
)
def test_timer_frequency(tmp_path, language, timer_header, override, expected):
    # Isolate macro selection from the real toolchain values, which may be equal.
    (tmp_path / "timer.h").write_text(timer_header)
    includes = Path(__file__).resolve().parents[1] / "lib_basix" / "api"
    command = ["xcc", "-E", "-x", language, "-I", str(tmp_path), "-I", str(includes)]
    if override is not None:
        command.append(f"-DBASIX_TIMER_HZ={override}U")

    source = "#include <basix/architecture.h>\n"
    if expected is not None:
        source += f"#if BASIX_TIMER_HZ != {expected}U\n#error Incorrect timer frequency\n#endif\n"
    source_file = tmp_path / "frequency.c"
    source_file.write_text(source)
    command.append(str(source_file))
    result = subprocess.run(
        command, capture_output=True, text=True, timeout=30,
    )

    if expected is None:
        assert result.returncode != 0
        assert "No toolchain timer frequency definition available" in result.stderr
    else:
        assert result.returncode == 0, result.stdout + result.stderr
