cmake_minimum_required(VERSION 3.0.0)
project(sake VERSION 0.1.0)

# Custom cmake modules
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

# Configure wether libraries will be static or shared linked
set(BUILD_SHARED_LIBS OFF)

set(SAKE_SOURCES
    sake/sake_array.c
    sake/sake_list.c
    sake/sake_vector.c
)

add_library(sake ${SAKE_SOURCES})
target_include_directories(sake PUBLIC sake)

enable_testing()
add_subdirectory(tests)