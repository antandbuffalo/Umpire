##############################################################################
# Copyright (c) 2018-2019, Lawrence Livermore National Security, LLC.
# Produced at the Lawrence Livermore National Laboratory
#
# Created by David Beckingsale, david@llnl.gov
# LLNL-CODE-747640
#
# All rights reserved.
#
# This file is part of Umpire.
#
# For details, see https://github.com/LLNL/Umpire
# Please also see the LICENSE file for MIT license.
##############################################################################
set (op_dir ${UMPIRE_TESTS_OUTPUT_DIR}/unit/op)

if (ENABLE_CUDA)
  blt_add_executable(
    NAME cuda_mem_advise_op_tests
    SOURCES cuda_mem_advise_op_tests.cpp
    DEPENDS_ON umpire cuda_runtime gtest gmock
    OUTPUT_DIR ${op_dir})

  blt_add_test(
    NAME cuda_mem_advise_op_tests
    COMMAND cuda_mem_advise_op_tests)
endif ()
