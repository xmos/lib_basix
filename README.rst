:orphan:

##############################
lib_basix: Basix library
##############################

:vendor: XMOS
:version: 0.1.0
:scope: General Use
:description: HAL and utility library
:category: General purpose
:keywords:
:devices: xcore-200, xcore.ai

*******
Summary
*******

Provides common C/XC utilities and a portability layer for XS and VX xcore architectures.

********
Features
********

* Portable assertion trap support
* Portable reference timer and delay APIs
* Basic print support via ``basix/print.h``
* C/XC compatibility definitions via ``basix/xccompat.h``
* xcore-400 shim mode and clock control via ``basix/shim.h``
* HAL support across XS and VX architectures

************
Known issues
************

* None

****************
Development repo
****************

* `lib_basix <https://www.github.com/xmos/lib_basix>`_ (https://www.github.com/xmos/lib_basix)

**************
Required tools
**************

* XMOS XTC Tools: 15.3.1

*********************************
Required libraries (dependencies)
*********************************

* None

*************************
Related application notes
*************************

* None

*******
Support
*******

This package is supported by XMOS Ltd. Issues can be raised against the software at
`www.xmos.com/support <https://www.xmos.com/support>`_ or using GitHub `issues <https://github.com/xmos/lib_basix/issues>`_.
