cmake_minimum_required(VERSION 2.25.1)
project(bo)

add_library(bo::bo IMPORTED INTERFACE)
target_include_directories(bo::bo INTERFACE include)

add_subdirectory(src)