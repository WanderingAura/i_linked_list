#include "eds_linked_list.hpp"
#include "gtest/gtest.h"
#include <csignal>
#include <gtest/gtest.h>

template<size_t PADDING_SIZE>
struct ListNode
{
    ListNode() :
        next{0},
        prev{0}
    {

    }
    ListNode(const ListNode&) = delete;

    ListNode& operator=(const ListNode&) = delete;
    size_t next;
    size_t prev;
    int value;
    size_t padding[PADDING_SIZE];
};

TEST(eds_LinkedList, node_obtain)
{
    constexpr int listSize = 1;
    eds::LinkedList<ListNode<0>> list(listSize);

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


TEST(eds_LinkedList, push_back)
{
    constexpr int listSize = 1;
    eds::LinkedList<ListNode<0>> list(listSize);

    auto& front = list.front();
    ASSERT_NE(front.value, 1);

    auto& node = list.node_obtain();

    node.value = 1;
    
    list.push_back(node);

    EXPECT_EQ(list.front().value, 1);
    EXPECT_EQ(list.back().value, 1);
}

TEST(eds_linked_list, push_back_lots)
{
    constexpr int listSize = 10;
    eds::LinkedList<ListNode<0>> list(listSize);

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

TEST(eds_linked_list, push_front)
{
    constexpr int listSize = 2;
    eds::LinkedList<ListNode<0>> list(listSize);

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