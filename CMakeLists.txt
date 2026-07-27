cmake_minimum_required(VERSION 3.25)
project(libyw C)

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

if(MSVC)
    add_compile_options(/W4 /WX /O2)
else()
    add_compile_options(
        -Wall -Wextra -Wpedantic -Werror
        -Wconversion -Wshadow -Wstrict-prototypes
        -fno-omit-frame-pointer
    )
    if(CMAKE_BUILD_TYPE STREQUAL "Release")
        add_compile_options(-O3 -march=native -flto)
    endif()
endif()

add_library(libyw STATIC
    src/string/string.c
    src/core/allocator.c
    src/container/vec.c
    src/math/math.c
)

target_include_directories(libyw PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

enable_testing()

add_executable(test_string tests/test_string.c)
target_link_libraries(test_string PRIVATE libyw)
add_test(NAME test_string COMMAND test_string)

add_executable(test_allocator tests/test_allocator.c)
target_link_libraries(test_allocator PRIVATE libyw)
add_test(NAME test_allocator COMMAND test_allocator)