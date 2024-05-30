.. _quickinstall:

===================
Quick Install Guide
===================

This quick install guide outlines the basic steps needed to install OpenSD on
your computer. 

Obtaining prerequisites on Ubuntu
---------------------------------

When building OpenSD from source, all :ref:`prerequisites <prerequisites>` can
be installed using the package manager:

.. code-block:: sh

    sudo apt install g++ cmake

After the packages have been installed, follow the instructions to build from
source below.

Building Source on Linux
------------------------

All OpenSD source code is hosted on `GitHub
<https://github.com/opensd-dev/opensd>`_. If you have `git
<https://git-scm.com>`_, a modern C++ compiler, and 
`CMake <https://cmake.org>`_ installed, you can
download and install OpenSD by entering the following commands in a terminal:

.. code-block:: sh

    git clone --recurse-submodules https://github.com/opensd-dev/opensd.git
    cd opensd
    mkdir build && cd build
    cmake ..
    make
    sudo make install

This will build an executable named ``opensd`` and install it (by default in
/usr/local/bin). If you do not have administrator privileges, the cmake command
should specify an installation directory where you have write access, e.g.

.. code-block:: sh

    cmake -DCMAKE_INSTALL_PREFIX=$HOME/.local ..

The :mod:`opensd` Python package must be installed separately. The easiest way
to install it is using `pip <https://pip.pypa.io/en/stable/>`_.
From the root directory of the OpenSD repository, run:

.. code-block:: sh

    python -m pip install .

