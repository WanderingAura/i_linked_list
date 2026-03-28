cmake_minimum_required(VERSION 3.11)

project(eds_linked_list)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (SSLL_BUILD_TESTS)
    enable_testing()
    add_subdirectory(test)
endif()