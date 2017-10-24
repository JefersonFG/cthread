cmake_minimum_required(VERSION 3.7)
project(testes)

set(CMAKE_C_STANDARD 99)

include_directories(../include)
link_directories(../lib)

set(SOURCE_FILES
        main.c
        ../src/scheduler.c
        ../src/cthread.c
        ../include/scheduler.h
        ../include/cthread.h
        ../include/cdata.h
        ../include/support.h
        ../bin/support.o manguaca.c)

add_executable(testes ${SOURCE_FILES})

#target_link_libraries(testes cthread)
set_target_properties(testes PROPERTIES COMPILE_FLAGS "-m32" LINK_FLAGS "-m32")