find_package(GTest REQUIRED)

add_executable(eds_linked_list_test eds_linked_list_test.cpp)
target_compile_options(eds_linked_list_test PRIVATE -g)
target_include_directories(eds_linked_list_test PRIVATE ..)

# NOTE: we need this to catch release build bugs
add_executable(eds_linked_list_test_fast eds_linked_list_test.cpp)
target_compile_options(eds_linked_list_test_fast PRIVATE -g -O2)
target_include_directories(eds_linked_list_test_fast PRIVATE ..)


target_link_libraries(eds_linked_list_test PRIVATE
    GTest::gtest_main
)
target_link_libraries(eds_linked_list_test_fast PRIVATE
    GTest::gtest_main
)

include(GoogleTest)

gtest_discover_tests(eds_linked_list_test)