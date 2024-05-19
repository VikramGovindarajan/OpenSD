.. _devguide_contributing:

======================
Contributing to OpenSD
======================
The general steps for contributing are as follows:

1. Fork the main opensd repository from `VikramGovindarajan/opensd`_. This will create a
   repository with the same name under your personal account. As such, you can
   commit to it as you please without disrupting other developers.

2. Clone your fork of OpenSD and create a branch that branches off of *develop*:

   .. code-block:: sh

       git clone --recurse-submodules git@github.com:yourusername/opensd.git
       cd opensd
       git checkout -b newbranch develop

3. Make your changes on the new branch that you intend to have included in
   *develop*. If you have made other changes that should not be merged back,
   ensure that those changes are made on a different branch.

4. Issue a pull request from GitHub and select the *develop* branch of
   VikramGovindarajan/opensd`_/opensd as the target.

   At a minimum, you should describe what the changes you've made are and why
   you are making them. If the changes are related to an outstanding issue, make
   sure it is cross-referenced.

5. Your pull request will be reviewed based on the criteria
   above. Any issues with the pull request can be discussed directly on the pull
   request page itself.

6. After the pull request has been thoroughly vetted, it is merged back into the
   *develop* branch of VikramGovindarajan/opensd.

Working in "Development" Mode
-----------------------------

If you are making changes to the Python API during development, it is highly
suggested to install the Python API in development/editable mode using
pip_. From the root directory of the OpenSD repository, run:

.. code-block:: sh

    python -m pip install -e .[test]

This installs the OpenSD Python package in `"editable" mode
<https://pip.pypa.io/en/stable/cli/pip_install/#editable-installs>`_ so that 1)
it can be imported from a Python interpreter and 2) any changes made are
immediately reflected in the installed version (that is, you don't need to keep
reinstalling it). 

.. _pip: https://pip.pypa.io/en/stable/
.. _VikramGovindarajan/opensd: https://github.com/VikramGovindarajan/opensd
