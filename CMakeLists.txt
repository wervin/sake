cmake_minimum_required(VERSION 3.10)
project(sake VERSION 0.1.0)

# Custom cmake modules
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

# Configure wether libraries will be static or shared linked
set(BUILD_SHARED_LIBS OFF)

set(SAKE_SOURCES
    src/sake/array.c
    src/sake/list.c
    src/sake/string.c
    src/sake/vector.c
)

add_library(sake ${SAKE_SOURCES})

target_include_directories(sake PUBLIC inc)

set_property(TARGET sake PROPERTY C_STANDARD 17)

target_compile_options(sake PRIVATE 
    -Wall -Wextra -Wpedantic)

enable_testing()
add_subdirectory(tests)

install(TARGETS sake)