cmake_minimum_required(VERSION 2.6)
project(sh4asm)
set(sh4asm_VERSION_MAJOR 0)
set(sh4asm_VERSION_MINOR 1)

enable_testing()

if(UNIX)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c11")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Werror")
elseif(MSVC)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /W4 /WX")
    # Static link to CRT in release mode
    set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} /MT")
    set(CMAKE_C_FLAGS_RELWITHDEBINFO "${CMAKE_C_FLAGS_RELWITHDEBINFO} /MT")
    set(CMAKE_C_FLAGS_MINSIZEREL "${CMAKE_C_FLAGS_MINSIZEREL} /MT")
endif()

if(MSVC)
    # Minimalist copy of MinGW-w64 to have features not available in MSVC
    set(mingw_w64_source
        "mingw_w64/err.h"
        "mingw_w64/getopt.h"
        "mingw_w64/err.c"
        "mingw_w64/getopt.c")

    include_directories("${PROJECT_SOURCE_DIR}/mingw_w64")
    add_library(mingw_w64 ${mingw_w64_source})
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

if(UNIX)
    target_link_libraries(sh4asm sh4asm_core)
elseif(MSVC)
    target_link_libraries(sh4asm sh4asm_core mingw_w64)
endif()

add_executable(sh4asm_test "${PROJECT_SOURCE_DIR}/test/sh4asm_test.c")
if(UNIX)
    target_link_libraries(sh4asm_test sh4asm_core)
elseif(MSVC)
    target_link_libraries(sh4asm_test sh4asm_core mingw_w64)
endif()
add_test(NAME sh4asm_test COMMAND ./sh4asm_test)
