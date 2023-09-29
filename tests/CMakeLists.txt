find_package(Check REQUIRED)

add_executable(check_sake check_sake.c)

target_link_libraries(check_sake sake ${CHECK_LIBRARIES})

set_property(TARGET check_sake PROPERTY C_STANDARD 17)

target_compile_options(check_sake PRIVATE 
    -Wall -Wextra -Wpedantic)

add_test(CheckSake check_sake)
