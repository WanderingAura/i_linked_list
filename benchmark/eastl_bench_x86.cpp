
#include <memory>
#include <iostream>
#include "EASTL/intrusive_list.h"
#include "integer_types.h"
#include <x86intrin.h>
#include <vector>
#include <random>
#include <algorithm>


template<size_t PADDING_SIZE>
struct ListNode : public eastl::intrusive_list_node
{
    ListNode() :
        eastl::intrusive_list_node()
    {
    }

    ListNode(const ListNode&) = delete;

    ListNode& operator=(const ListNode&) = delete;
    u8 padding[PADDING_SIZE];
};

template <size_t PADDING_BYTES>
void benchmark_linear_insertion(long num_elements, int num_loops)
{

    long sum = 0;
    u64 total_cycles = 0;
    for (int j = 0; j < num_loops; j++)
    {
        auto nodes = std::make_unique<ListNode<PADDING_BYTES>[]>(num_elements);
        for (int i = 0; i < num_elements; i++)
        {
            nodes[i].padding[0] = rand();
        }
        eastl::intrusive_list<ListNode<PADDING_BYTES>> list;

        std::vector<size_t> indexes(num_elements);
        for (int i = 0; i < num_elements; ++i)
        {
            indexes[i] = i;
        }

        u64 start = __rdtsc();
        for (auto& index : indexes)
        {
            list.push_back(nodes[index]);
        }
        u64 end = __rdtsc();

        u64 cycles = end - start;
        total_cycles += cycles;


        for (int i = 0; i < num_elements; i++)
        {

            auto& node = list.back();
            list.pop_back();
            sum += node.padding[0];
        }
    }
    double cyclesPerElement = (double)total_cycles / (double)(num_elements * num_loops);
    long bytesKB = num_elements * sizeof(ListNode<PADDING_BYTES>) / 1024;
    std::cout << __FUNCTION__ << " padding bytes: " << PADDING_BYTES << ", Cycles per element: " << cyclesPerElement << ", kb: " << bytesKB <<  ", sum: " << sum << "\n";
}

template <size_t PADDING_BYTES>
void benchmark_random_insertion(long num_elements, int num_loops)
{

    std::random_device rd;
    std::mt19937 rng(rd());
    long sum = 0;
    u64 total_cycles = 0;
    for (int j = 0; j < num_loops; j++)
    {
        auto nodes = std::make_unique<ListNode<PADDING_BYTES>[]>(num_elements);
        for (int i = 0; i < num_elements; i++)
        {
            nodes[i].padding[0] = rand();
        }
        eastl::intrusive_list<ListNode<PADDING_BYTES>> list;
        std::vector<size_t> indexes(num_elements);
        for (int i = 0; i < num_elements; ++i)
        {
            indexes[i] = i;
        }
        std::shuffle(indexes.begin(), indexes.end(), rng);

        u64 start = __rdtsc();
        for (auto& index : indexes)
        {
            list.push_back(nodes[index]);
        }
        u64 end = __rdtsc();

        u64 cycles = end - start;
        total_cycles += cycles;


        for (int i = 0; i < num_elements; i++)
        {

            auto& node = list.back();
            list.pop_back();
            sum += node.padding[0];
        }
    }
    double cyclesPerElement = (double)total_cycles / (double)(num_elements * num_loops);
    long bytesKB = num_elements * sizeof(ListNode<PADDING_BYTES>) / 1024;
    std::cout << __FUNCTION__ << " padding bytes: " << PADDING_BYTES << ", Cycles per element: " << cyclesPerElement << ", kb: " << bytesKB <<  ", sum: " << sum << "\n";
}

template <size_t PADDING_BYTES>
void benchmark_linear_access(long num_elements, int num_loops)
{

    std::random_device rd;
    std::mt19937 rng(rd());
    long sum = 0;
    u64 total_cycles = 0;
    for (int j = 0; j < num_loops; j++)
    {
        auto nodes = std::make_unique<ListNode<PADDING_BYTES>[]>(num_elements);
        for (int i = 0; i < num_elements; i++)
        {
            nodes[i].padding[0] = rand();
        }
        eastl::intrusive_list<ListNode<PADDING_BYTES>> list;
        std::vector<size_t> indexes(num_elements);
        for (int i = 0; i < num_elements; ++i)
        {
            indexes[i] = i;
        }

        for (auto& index : indexes)
        {
            list.push_back(nodes[index]);
        }


        u64 start = __rdtsc();
        for (int i = 0; i < num_elements; i++)
        {

            auto& node = list.back();
            list.pop_back();
            sum += node.padding[0];
        }
        u64 end = __rdtsc();

        u64 cycles = end - start;
        total_cycles += cycles;
    }
    double cyclesPerElement = (double)total_cycles / (double)(num_elements * num_loops);
    long bytesKB = num_elements * sizeof(ListNode<PADDING_BYTES>) / 1024;
    std::cout << __FUNCTION__ << " padding bytes: " << PADDING_BYTES << ", Cycles per element: " << cyclesPerElement << ", kb: " << bytesKB <<  ", sum: " << sum << "\n";
}

template <size_t PADDING_BYTES>
void benchmark_random_access(long num_elements, int num_loops)
{

    std::random_device rd;
    std::mt19937 rng(rd());
    long sum = 0;
    u64 total_cycles = 0;
    for (int j = 0; j < num_loops; j++)
    {
        auto nodes = std::make_unique<ListNode<PADDING_BYTES>[]>(num_elements);
        for (int i = 0; i < num_elements; i++)
        {
            nodes[i].padding[0] = rand();
        }
        eastl::intrusive_list<ListNode<PADDING_BYTES>> list;
        std::vector<size_t> indexes(num_elements);
        for (int i = 0; i < num_elements; ++i)
        {
            indexes[i] = i;
        }
        std::shuffle(indexes.begin(), indexes.end(), rng);

        for (auto& index : indexes)
        {
            list.push_back(nodes[index]);
        }


        u64 start = __rdtsc();
        for (int i = 0; i < num_elements; i++)
        {

            auto& node = list.back();
            list.pop_back();
            sum += node.padding[0];
        }
        u64 end = __rdtsc();

        u64 cycles = end - start;
        total_cycles += cycles;
    }
    double cyclesPerElement = (double)total_cycles / (double)(num_elements * num_loops);
    long bytesKB = num_elements * sizeof(ListNode<PADDING_BYTES>) / 1024;
    std::cout << __FUNCTION__ << " padding bytes: " << PADDING_BYTES << ", Cycles per element: " << cyclesPerElement << ", kb: " << bytesKB <<  ", sum: " << sum << "\n";
}
constexpr size_t paddings[] = {1, 4, 8, 14, 16, 24, 32, 40, 48, 56, 64, 65, 68, 72, 80, 88, 128, 256, 512, 1024};
template<std::size_t... Is>
void run_all(std::index_sequence<Is...>, u64 num_elements, u64 num_loops)
{
    // (benchmark_linear_insertion<paddings[Is]>(num_elements, num_loops), ...);
    // (benchmark_random_insertion<paddings[Is]>(num_elements, num_loops), ...);
    (benchmark_linear_access<paddings[Is]>(num_elements, num_loops), ...);
    (benchmark_random_access<paddings[Is]>(num_elements, num_loops), ...);
}

int main(int argc, char** argv)
{
    if (argc != 2)
    {
        std::cout << "Usage: " << argv[0] << " <number_of_elements>\n";
        return 1;
    }

    int num_elements = std::atoi(argv[1]);

    int num_loops = 16;
    run_all(std::make_index_sequence<std::size(paddings)>(), num_elements, num_loops);
}
