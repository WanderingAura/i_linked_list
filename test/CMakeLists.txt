find_package(GTest REQUIRED)

add_executable(ill_linked_list_test ill_linked_list_test.cpp)
target_compile_options(ill_linked_list_test PRIVATE -g)
target_include_directories(ill_linked_list_test PRIVATE ..)

add_executable(ill_linked_list_test_fast ill_linked_list_test.cpp)
target_compile_options(ill_linked_list_test_fast PRIVATE -g -O2)
target_include_directories(ill_linked_list_test_fast PRIVATE ..)


target_link_libraries(ill_linked_list_test PRIVATE
    GTest::gtest_main
)
target_link_libraries(ill_linked_list_test_fast PRIVATE
    GTest::gtest_main
)

include(GoogleTest)

gtest_discover_tests(ill_linked_list_test)
