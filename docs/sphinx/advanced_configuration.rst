.. _advanced_configuration:

======================
Advanced Configuration
======================

In addition to the normal options provided by CMake, Umpire uses some additional
configuration arguments to control optional features and behavior. Each
argument is a boolean option, and  can be turned on or off:

.. code-block:: bash

    -DENABLE_CUDA=Off

Here is a summary of the configuration options, their default value, and meaning:

      ===========================  ======== ===============================================================================
      Variable                     Default  Meaning
      ===========================  ======== ===============================================================================
      ``ENABLE_CUDA``              On       Enable CUDA support
      ``ENABLE_TESTING``           On       Build test executables
      ``ENABLE_BENCHMARKS``        On       Build benchmark programs
      ===========================  ======== ===============================================================================

These arguments are explained in more detail below:

* ``ENABLE_CUDA``
  This option enables support for GPUs. If CHAI is built without CUDA support,
  then only the ``CPU`` execution space is available for use.

* ``ENABLE_TESTING``
  This option controls whether or not test executables will be built.

* ``ENABLE_BENCHMARKS``
  This option will build the benchmark programs used to test ``ManagedArray``
  performance.
