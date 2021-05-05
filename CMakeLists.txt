cmake_minimum_required(VERSION 3.17)
project(MyShell C)

set(CMAKE_C_STANDARD 99)

add_executable(MyShell main.c)

add_custom_target(MyShell COMMAND make -C ${MyShell_SOURCE_DIR}
        CLION_EXE_DIR=${PROJECT_BINARY_DIR})