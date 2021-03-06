# Copyright (c) 2018 by contributors.
# Authour: Chao Ma (mctt90@gmail.com)
#
# This is the root CMakeLists.txt file of the xForest project, which
# build all subdirectories (packages) in the order of
# inter-package dependence.
#
# you can build the xForest using the following commands:
#
# $> cd xforest
# $> mkdir build
# $> cd build
# $> cmake ..
# $> make
#
# Thus you check out the xforest project and build it in a
# subdirectory ``build". If you want further to install the
# built project, you can type the command:
#
# $> make install
#
project ("xforest")

cmake_minimum_required(VERSION 2.8)

#-------------------------------------------------------------------------------
# Take almost all warnings;
# Do not generate debug symbols;
# Optimization level 3;
# Using c++11;
# Optimization on architecture
#-------------------------------------------------------------------------------
add_definitions("-Wall -Wno-sign-compare -O3 -std=c++11 
-march=native -Wno-strict-aliasing -Wno-comment")

#-------------------------------------------------------------------------------
# Declare where our project will be installed.
#-------------------------------------------------------------------------------
set(CMAKE_INSTALL_PREFIX "xforest")

#-------------------------------------------------------------------------------
# Set include paths and library paths.
#-------------------------------------------------------------------------------
include_directories(
  "${PROJECT_SOURCE_DIR}"
  "${PROJECT_BINARY_DIR}"
  "${PROJECT_SOURCE_DIR}/gtest/include"
)

link_directories(
  "${PROJECT_BINARY_DIR}/src/base"
  "${PROJECT_BINARY_DIR}/src/tree"
  "${PROJECT_BINARY_DIR}/src/reader"
  "${PROJECT_BINARY_DIR}/src/network"
  "${PROJECT_BINARY_DIR}/src/solver"
  "${PROJECT_BINARY_DIR}/src/c_api"
)

#-------------------------------------------------------------------------------
# Declare packages in xForest project.
#-------------------------------------------------------------------------------
add_subdirectory(gtest)
add_subdirectory(demo)
add_subdirectory(scripts)
add_subdirectory(src/base)
add_subdirectory(src/tree)
add_subdirectory(src/reader)
add_subdirectory(src/network)
add_subdirectory(src/solver)
add_subdirectory(src/c_api)
add_subdirectory(python-package)