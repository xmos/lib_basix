cmake_minimum_required(VERSION 3.21)
include($ENV{XMOS_CMAKE_PATH}/xcommon.cmake)

project(test_basix_fail)

set(APP_INCLUDES src)

if(NOT DEFINED APP_HW_TARGET)
  set(APP_HW_TARGET XK-EVK-XU316)
endif()

# The trap must remain active in release builds with assertions disabled.
set(APP_COMPILER_FLAGS          -O3 -Wall -DNDEBUG)

set(APP_DEPENDENT_MODULES       "lib_basix")

set(XMOS_SANDBOX_DIR ${CMAKE_CURRENT_LIST_DIR}/../../..)

XMOS_REGISTER_APP()
