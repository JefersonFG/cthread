cmake_minimum_required(VERSION 3.7)
project(tests)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -m32")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -m32")

include_directories(../include)
link_directories(../lib)

include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

set(SOURCE_FILES
        unit_tests.cpp
        ../src/scheduler.c
        ../src/cthread.c
        ../include/scheduler.h
        ../include/cthread.h
        ../include/cdata.h
        ../include/support.h
        ../bin/support.o)

add_executable(tests ${SOURCE_FILES})

target_link_libraries(tests gtest gtest_main)

set_target_properties(tests PROPERTIES COMPILE_FLAGS "-m32" LINK_FLAGS "-m32")

add_subdirectory(gtest)
