#include <cstddef>
#include <iostream>
#include "eds_linked_list.hpp"
#include <vector>
#include <cstdlib>
#include <x86intrin.h>
#include "integer_types.h"

template<typename index_t, size_t PADDING_SIZE>
struct ListNode
{
    ListNode()
    {

    }
    ListNode(const ListNode&) = delete;

    ListNode& operator=(const ListNode&) = delete;
    index_t next;
    index_t prev;
    u8 padding[PADDING_SIZE];
};

volatile int sink;

int main(int argc, char** argv)
{
    constexpr int padding_size = 6;
    constexpr long num_elements = 1024*1024;
    static_assert(sizeof(ListNode<u32,padding_size>) * num_elements < 2*1024*1024*1024U);
    std::vector<u16> values(num_elements);

    for (int i = 0; i < num_elements; i++)
    {
        values[i] = rand();
    }

    eds::LinkedList<ListNode<u32, padding_size>, u32> list(num_elements);
    for (u32 i = 0; i < 20; i++)
    {

        u64 start = __rdtsc();
        for (int i = 0; i < num_elements; i++)
        {
            auto& node = list.node_obtain();
            node.padding[0] = values[i];
            list.push_back(node);
        }

        u64 end = __rdtsc();

        u64 cycles = end - start;
        double cyclesPerElement = (double)cycles / (double)num_elements;

        std::cout << "Cycles per element: " << cyclesPerElement << "\n";

        for (int i = 0; i < num_elements; i++)
        {
            auto& node = list.pop_back();
            asm volatile("" : : "r"(&node) : "memory");
        }
    }

    return 0;
}
