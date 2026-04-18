# add_executable(eds_linked_list_benchmark eds_linked_list_bench_x86.cpp)
#
# target_compile_options(eds_linked_list_benchmark PRIVATE -g -O2)
# # target_compile_definitions(eds_linked_list_benchmark PRIVATE NDEBUG)
# target_include_directories(eds_linked_list_benchmark PRIVATE ..)

add_executable(comparison_benchmark comparison_bench.cpp)

target_compile_options(comparison_benchmark PRIVATE -g -O2)
target_include_directories(comparison_benchmark PRIVATE ..)
target_link_libraries(comparison_benchmark PRIVATE EASTL)
target_compile_definitions(comparison_benchmark PRIVATE NDEBUG)
add_subdirectory(EASTL)

