
#include <memory>
#include <iostream>
#include "EASTL/intrusive_list.h"
#include "integer_types.h"
#include <x86intrin.h>


template<size_t PADDING_SIZE>
struct ListNode : public eastl::intrusive_list_node
{
    ListNode() :
        eastl::intrusive_list_node()
    {
    }

    ListNode(const ListNode&) = delete;

    ListNode& operator=(const ListNode&) = delete;
    size_t padding[PADDING_SIZE];
};

int main(void)
{
    constexpr long num_elements = 1024*1024;
    auto nodes = std::make_unique<ListNode<1>[]>(num_elements);
    for (int i = 0; i < num_elements; i++)
    {
        nodes[i].padding[0] = rand();
    }

    eastl::intrusive_list<ListNode<1>> list;

    u64 start = __rdtsc();
    for (int i = 0; i< num_elements; i++)
    {
        list.push_back(nodes[i]);
    }
    u64 end = __rdtsc();

    u64 cycles = end - start;
    double cyclesPerElement = (double)cycles / (double)num_elements;

    std::cout << "Cycles per element: " << cyclesPerElement << "\n";

    for (int i = 0; i < num_elements; i++)
    {

        auto& node = list.back();
        list.pop_back();
        asm volatile("" : : "r"(&node) : "memory");
    }
}
