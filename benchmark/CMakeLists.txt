add_executable(eds_linked_list_benchmark eds_linked_list_bench_x86.cpp)

target_compile_options(eds_linked_list_benchmark PRIVATE -g -O2)
# target_compile_definitions(eds_linked_list_benchmark PRIVATE NDEBUG)
target_include_directories(eds_linked_list_benchmark PRIVATE ..)

add_executable(eastl_bench_x86 eastl_bench_x86.cpp)

target_compile_options(eastl_bench_x86 PRIVATE -g -O2)
target_include_directories(eastl_bench_x86 PRIVATE ..)
target_link_libraries(eastl_bench_x86 PRIVATE EASTL)
add_subdirectory(EASTL)

