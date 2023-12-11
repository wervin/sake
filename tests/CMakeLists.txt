add_executable(array array.c)

target_link_libraries(array sake ${CHECK_LIBRARIES})

set_property(TARGET array PROPERTY C_STANDARD 17)

target_compile_options(array PRIVATE 
    -Wall -Wextra -Wpedantic)

add_test(test_sake_array array)


add_executable(list list.c)

target_link_libraries(list sake ${CHECK_LIBRARIES})

set_property(TARGET list PROPERTY C_STANDARD 17)

target_compile_options(list PRIVATE 
    -Wall -Wextra -Wpedantic)

add_test(test_sake_list list)


add_executable(string string.c)

target_link_libraries(string sake ${CHECK_LIBRARIES})

set_property(TARGET string PROPERTY C_STANDARD 17)

target_compile_options(string PRIVATE 
    -Wall -Wextra -Wpedantic)

add_test(test_sake_string string)


add_executable(vector vector.c)

target_link_libraries(vector sake ${CHECK_LIBRARIES})

set_property(TARGET vector PROPERTY C_STANDARD 17)

target_compile_options(vector PRIVATE 
    -Wall -Wextra -Wpedantic)

add_test(test_sake_vector vector)
