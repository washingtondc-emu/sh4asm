cmake_minimum_required(VERSION 2.6)
project(sh4asm)
set(sh4asm_VERSION_MAJOR 0)
set(sh4asm_VERSION_MINOR 1)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c11")

enable_testing()

# turn on strict warnings - i have no idea how to do this in windows
if(UNIX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror")
endif()

set(sh4asm_core_sources
    "sh4asm_core/sh4_opcodes.h"
    "sh4asm_core/sh4_bin_emit.h"
    "sh4asm_core/sh4_bin_emit.c"
    "sh4asm_core/sh4_asm_emit.h"
    "sh4asm_core/sh4_asm_emit.c"
    "sh4asm_core/lexer.c"
    "sh4asm_core/lexer.h"
    "sh4asm_core/parser.c"
    "sh4asm_core/parser.h"
    "sh4asm_core/sh4asm.h"
    "sh4asm_core/sh4asm.c"
    "sh4asm_core/disas.c"
    "sh4asm_core/disas.h")

include_directories("${PROJECT_SOURCE_DIR}/sh4asm_core")

add_library(sh4asm_core ${sh4asm_core_sources})

add_executable(sh4asm "sh4asm/main.c")
target_link_libraries(sh4asm sh4asm_core)

add_executable(sh4asm_test "${PROJECT_SOURCE_DIR}/test/sh4asm_test.c")
target_link_libraries(sh4asm_test sh4asm_core)
add_test(NAME sh4asm_test COMMAND ./sh4asm_test)