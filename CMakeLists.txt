cmake_minimum_required(VERSION 2.8)
project(cresus)

# src for cresus project

# subdirs
add_subdirectory(
  framework
  engine
  indicator
  input
  math
  module
  program
  sample
  sim
  trading
  utest)

# includes
include_directory(${PROJECT_SOURCE_DIR}/framework)
