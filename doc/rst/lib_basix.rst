########################
lib_basix: Basix library
########################


*****
Usage
*****

``lib_basix`` is intended to be used with the `XCommon CMake <https://www.xmos.com/file/xcommon-cmake-documentation/?version=latest>`_
, the `XMOS` application build and dependency management system.

To use this library in an application include ``lib_basix`` in the application's ``APP_DEPENDENT_MODULES`` list in
`CMakeLists.txt`, for example:

.. code-block:: cmake

    set(APP_DEPENDENT_MODULES "lib_basix")

.. note:: Dependent modules should be pinned to release versions where possible, otherwise the
   latest commit on the `develop` branch will be used.  For further details on managing modules,
   pinning to a release version and other options, please see the page `xcommon-cmake Dependency Management <https://www.xmos.com/documentation/XM-015090-PC/html/doc/dependency_management.html>`_.

All ``lib_basix`` functions can be accessed via the ``basix.h`` header file, for example:

.. code-block:: C

    #include "basix.h"

*******************
Example application
*******************

Building the example
====================

This section assumes that the `XMOS XTC Tools <https://www.xmos.com/software-tools/>`_ have been
downloaded and installed. The required version is specified in the accompanying ``README``.

Installation instructions can be found `here <https://xmos.com/xtc-install-guide>`_.

Special attention should be paid to the section on
`Installation of Required Third-Party Tools <https://www.xmos.com/documentation/XM-014363-PC/html/installation/install-configure/install-tools/install_prerequisites.html>`_.

The application is built using the `xcommon-cmake <https://www.xmos.com/file/xcommon-cmake-documentation/?version=latest>`_
build system, which is provided with the XTC tools and is based on `CMake <https://cmake.org/>`_.

The ``lib_basix`` software ZIP package should be downloaded and extracted to a chosen working
directory.

To configure the build, the following commands should be run from an XTC command prompt:

.. code-block:: bash

    cd lib_basix/examples/app_basix
    cmake -G "Unix Makefiles" -B build

If any dependencies are missing they will be retrieved automatically during this step.

The application binaries should then be built using ``xmake``:

.. code-block:: bash

    xmake -j -C build

Binary artifacts (.xe files) will be generated under the appropriate subdirectories of the
``app_basix/bin`` directory — one for each supported build configuration.

For subsequent builds, the ``cmake`` step may be omitted.
If ``CMakeLists.txt`` or other build files are modified, ``cmake`` will be re-run automatically
by ``xmake`` as needed.

Running the example
===================

From an XTC command prompt, the following command should be run from the ``examples/app_basix``
directory:

.. code-block:: bash

    xrun ./bin/app_basix.xe

Alternatively, the application can be programmed into flash memory for standalone execution:

.. code-block:: bash

   xflash ./bin/app_basix.xe

****************
Shim control API
****************

The ``basix/shim.h`` header provides functions for configuring or disabling the
shim control register on xcore-400 devices. It defines these operating modes:

* ``SHIM_MODE_DISABLED``: disable the shim and connect pads directly to their ports.
* ``SHIM_MODE_I3C``: configure the shim for I3C.
* ``SHIM_MODE_SOUNDWIRE_MASTER`` and ``SHIM_MODE_SOUNDWIRE_SLAVE``: configure the
  SoundWire role.

The clock source can be selected with ``SHIM_CLOCK_SOC`` (PLL0) or
``SHIM_CLOCK_DDR`` (the DDR controller).

Enable the configuration port before calling the shim API. ``shim_enable``
configures and applies the selected mode and clock. ``shim_disable`` selects the
disabled mode; it does not disable the port itself.

.. code-block:: C

    #include <xs1.h>
    #include <xcore/port.h>
    #include <basix/shim.h>

    void configure_shim(void)
    {
        const port_t p_shim_config = XS1_PORT_4C;
        port_enable(p_shim_config);
        shim_enable(SHIM_MODE_SOUNDWIRE_MASTER, SHIM_CLOCK_SOC, p_shim_config);
        <Use the shim>
        shim_disable(p_shim_config);
        port_disable(p_shim_config);
    }

For timing-sensitive use, split configuration from application: call
``shim_configure`` to shift in the selected mode and clock, then call
``shim_latch`` when the configuration should be applied. Only one configuration
may be pending, so call ``shim_latch`` before another ``shim_configure``.

.. code-block:: C

    const port_t p_shim_config = XS1_PORT_4C;
    port_enable(p_shim_config);
    shim_configure(SHIM_MODE_SOUNDWIRE_SLAVE, SHIM_CLOCK_DDR, p_shim_config);
    /* Synchronize with the relevant clock here. */
    shim_latch(p_shim_config);
    port_disable(p_shim_config);

*************
API Reference
*************

Public declarations, parameters, and enum values are documented in the Doxygen
comments in ``basix/shim.h`` and the other public headers.



