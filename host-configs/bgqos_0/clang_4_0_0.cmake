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
set(CMAKE_CXX_COMPILER "/usr/apps/gnu/clang/r284961-stable/bin/bgclang++11" CACHE PATH "")
set(CMAKE_C_COMPILER "/usr/apps/gnu/clang/r284961-stable/bin/bgclang" CACHE PATH "")

set(CMAKE_CXX_FLAGS "-stdlib=libc++" CACHE STRING "")
