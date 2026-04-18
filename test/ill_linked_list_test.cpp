#include "ill_linked_list.hpp"
#include "gtest/gtest.h"
#include <csignal>
#include <gtest/gtest.h>
#include "integer_types.h"

template<typename index_t, size_t PADDING_SIZE>
struct ListNodeIList
{
    ListNodeIList()
    {

    }
    ListNodeIList(const ListNodeIList&) = delete;

    ListNodeIList& operator=(const ListNodeIList&) = delete;
    index_t next;
    index_t prev;
    u8 padding[PADDING_SIZE];
};

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
    int value;
    u8 padding[PADDING_SIZE];
};

TEST(ill, node_obtain)
{
    constexpr int listSize = 1;
    ill::LinkedList<ListNode<u32, 1>, u32> list(listSize);

    auto& front = list.front();
    EXPECT_EQ(front.prev, 0);
    EXPECT_EQ(front.next, 0);

    auto& node = list.node_obtain();

    ASSERT_NE(&front, &node);
    EXPECT_EQ(node.prev, 0);
    EXPECT_EQ(node.next, 0);

    // TODO: we should support increasing the number of nodes
    EXPECT_EXIT(
        {
            auto& node = list.node_obtain(); // we're out of nodes!
        },
        testing::KilledBySignal(SIGABRT),
        "Assertion .* failed"
    );
}


TEST(ill_LinkedList, push_back)
{
    constexpr int listSize = 1;
    ill::LinkedList<ListNode<u32, 1>, uint32_t> list(listSize);

    auto& front = list.front();
    ASSERT_NE(front.value, 1);

    auto& node = list.node_obtain();

    node.value = 1;
    
    list.push_back(node);

    EXPECT_EQ(list.front().value, 1);
    EXPECT_EQ(list.back().value, 1);
}

TEST(ill_linked_list, push_back_lots)
{
    constexpr int listSize = 10;
    ill::LinkedList<ListNode<u32, 1>, uint32_t> list(listSize);

    for (size_t i = 0; i < listSize; i++)
    {

        auto& node = list.node_obtain();
        node.value = i+1;
        list.push_back(node);

        ASSERT_EQ(list.back().value, i+1);
    }
    EXPECT_EQ(list.front().value, 1);

    auto it = list.begin();
    size_t count = 1;
    while (it != list.end())
    {
        ASSERT_EQ((*it).value, count);
        ++count;
        ++it;
    }
}

TEST(ill_linked_list, push_back_u16)
{
    constexpr int listSize = 10;
    ill::LinkedList<ListNode<u16, 1>, u16> list(listSize);

    for (size_t i = 0; i < listSize; i++)
    {

        auto& node = list.node_obtain();
        node.value = i+1;
        list.push_back(node);

        ASSERT_EQ(list.back().value, i+1);
    }
    EXPECT_EQ(list.front().value, 1);

    auto it = list.begin();
    size_t count = 1;
    while (it != list.end())
    {
        ASSERT_EQ((*it).value, count);
        ++count;
        ++it;
    }
}

TEST(ill_linked_list, push_front)
{
    constexpr int listSize = 2;
    ill::LinkedList<ListNode<u32, 1>, uint32_t> list(listSize);

    auto& node1 = list.node_obtain();

    node1.value = 1;
    
    list.push_front(node1);

    EXPECT_EQ(list.front().value, 1);
    EXPECT_EQ(list.back().value, 1);

    auto& node2 = list.node_obtain();
    node2.value = 2;
    list.push_front(node2);

    EXPECT_EQ(list.front().value, 2);
    EXPECT_EQ(list.back().value, 1);
}
