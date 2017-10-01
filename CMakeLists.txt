cmake_minimum_required(VERSION 3.7)
project(libcthread.a)

set(CMAKE_C_STANDARD 99)

set(SOURCE_FILES src/insert.c include/cdata.h include/cthread.h include/support.h)

add_library(libcthread.a ${SOURCE_FILES} bin/support.o)

# Run standard makefile
# add_custom_target(cthread ALL COMMAND make WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})