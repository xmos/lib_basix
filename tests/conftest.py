# Copyright 2025-2026 XMOS LIMITED.
# This Software is subject to the terms of the XMOS Public Licence: Version 1.
from pathlib import Path

import pytest


def get_app_hw_target(build_dir):
    cache_file = build_dir / "CMakeCache.txt"
    if not cache_file.is_file():
        return None

    for line in cache_file.read_text().splitlines():
        if line.startswith("APP_HW_TARGET:"):
            return line.partition("=")[2]
    return None


def should_ignore_shim_test_for_target(collection_path, tests_dir):
    if collection_path.name != "test_basix_shim.py":
        return None

    for build_dir_name in ("build", "build-xs"):
        target = get_app_hw_target(tests_dir / build_dir_name)
        if target == "XK-EVK-XU416":
            return False
        if target == "XK-EVK-XU316":
            return True
    return None


def pytest_ignore_collect(collection_path, config):
    return should_ignore_shim_test_for_target(
        collection_path,
        Path(__file__).resolve().parent,
    )


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
