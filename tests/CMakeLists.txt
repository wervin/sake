find_package(Check REQUIRED)

add_executable(check_sake check_sake.c)
target_link_libraries(check_sake sake ${CHECK_LIBRARIES})

add_test(CheckSake check_sake)
