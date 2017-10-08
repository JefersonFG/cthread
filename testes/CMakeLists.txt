cmake_minimum_required(VERSION 3.8)
project(testes)

set(CMAKE_C_STANDARD 99)

include_directories(../include)
link_directories(../lib)

set(SOURCE_FILES main.c)
add_executable(testes ${SOURCE_FILES})

target_link_libraries(testes cthread)
set_target_properties(testes PROPERTIES COMPILE_FLAGS "-m32" LINK_FLAGS "-m32")