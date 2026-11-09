cmake_minimum_required(VERSION 3.21)
include($ENV{XMOS_CMAKE_PATH}/xcommon.cmake)
project(tests)

# XC sources require the XS/XTC 15 toolchain and cannot be built for VX targets.
option(BASIX_BUILD_XC_TESTS "Build XS-only XC compatibility tests" OFF)

add_subdirectory(test_basix_pass)
if(APP_HW_TARGET STREQUAL "XK-EVK-XU416")
  add_subdirectory(test_basix_shim)
endif()
add_subdirectory(test_basix_fail)

if(BASIX_BUILD_XC_TESTS)
  add_subdirectory(test_basix_xc)
endif()
