#include <cstddef>
#include <cassert>

#ifndef NDEBUG
#include <cstdio>
#define DBG_PRINT(...) printf(__VA_ARGS__)
#else
#define DBG_PRINT(...)
#endif

namespace eds
{

template <typename T, typename index_t>
class LinkedListIterator
{
public:
    LinkedListIterator(T* data, index_t index) :
        m_data(data),
        m_index(index)
    {

    }

    T& operator*()
    {
        return m_data[m_index];
    }

    T* operator->()
    {
        return m_data + m_index;
    }

    const T* operator->() const
    {
        return m_data + m_index;
    }

    T& operator++() // prefix
    {
        m_index = m_data[m_index].next;
        return m_data[m_index];
    }

    bool operator==(const LinkedListIterator<T, index_t>& rhs)
    {
        return rhs.m_data == m_data &&
            rhs.m_index == m_index;
    }

    bool operator!=(const LinkedListIterator<T, index_t>& rhs)
    {
        return !operator==(rhs);
    }

    LinkedListIterator<T, index_t> operator++(int) // postfix
    {
        LinkedListIterator<T, index_t> temp = *this;
        ++(*this);
        return temp;
    }


private:
    T* m_data;
    index_t m_index;
};

template <typename T, typename index_t>
class LinkedList
{
public:
    LinkedList() = default;
    LinkedList(index_t capacity) :
        m_data(new T[capacity]),
        m_free{1},
        m_capacity(capacity)
    {
        m_data = new T[capacity+1];

        // set up the free list
        for (index_t i = 1; i < capacity+1; i++)
        {
            m_data[i].next = (i+1) % (capacity+1);
        }
        m_data[0].next = 0;
        m_data[0].prev = 0;
    }

    // NOTE: is this linked list even a good idea?
    // We really want to be able to support types T that aren't default constructible...
    // This isn't possible really if we allocate inside the structure.
    // What if we just let the user decide on how to arrange the data, like a normal
    // library? The user allocates and then the library handles the data structure.
    // Will this even be harder on performance? I don't think so
    // Well if the user decides to not use an array, then we can't use indexes.
    // I think I will make this a niche class that allocates the data and requires
    // the type to be default constructible.

    ~LinkedList()
    {
        delete [] m_data;
    }

    T& front()
    {
        index_t head = m_data->next;
        return m_data[head];
    }

    T& back()
    {
        index_t tail = m_data->prev;
        return m_data[tail];
    }

    LinkedListIterator<T, index_t> begin()
    {
        return LinkedListIterator<T, index_t>(m_data, m_data->next);
    }

    LinkedListIterator<T,index_t> end()
    {
        return LinkedListIterator<T, index_t>(m_data, SENTINEL_IDX);
    }

    void remove(T& toRemove)
    {
        // NOTE: cool idea from EASTL: by using the sentinel value at index 0 to store
        // the start and end of the list we remove the need to branch.
        m_data[toRemove.prev].next = toRemove.next;
        m_data[toRemove.next].prev = toRemove.prev;

#ifndef NDEBUG
        toRemove.next = SENTINEL_IDX;
        toRemove.prev = SENTINEL_IDX;
#endif
    }

    T& pop_back()
    {
        index_t backIdx = m_data->prev;
        T& back = m_data[backIdx];

        m_data->prev = back.prev;
        T& newBack = m_data[back.prev];
        newBack.next = SENTINEL_IDX;
        back.next = m_free;
        m_free = backIdx;
        return back;
    }

    T& pop_front()
    {
        // TODO: consider using remove() instead of code duplication here?
        // Will this improve the i-cache performance?
        index_t frontIdx = m_data->next;
        T& front = m_data[frontIdx];
        m_data->next = front.next;
        T& newFront = m_data[front.next];
        newFront.prev = SENTINEL_IDX;

        front.next = m_free;
        m_free = frontIdx;
    }

    void push_back(T& item)
    {
        // We need to confirm that the user is using an item allocated by us.
        // This check should be enough as long as the user hasn't reinterpret
        // casted something in the middle of T into T&
        assert(&item > m_data && &item <= m_data + m_capacity);
        T& back = m_data[m_data->prev];

        back.next = (&item - m_data);
        item.next = SENTINEL_IDX;
        item.prev = m_data->prev;

        m_data->prev = back.next;
    }

    void push_front(T& item)
    {
        // We need to confirm that the user is using an item allocated by us.
        // This check should be enough as long as the user hasn't reinterpret
        // casted something in the middle of T into T&
        assert(&item > m_data && &item <= m_data + m_capacity);
        // is there a better way of getting this index?
        index_t newFrontIdx = (&item - m_data);
        index_t oldFrontIdx = m_data->next;

        T& oldFront = m_data[oldFrontIdx];

        item.next = oldFrontIdx;
        item.prev = SENTINEL_IDX;
        oldFront.prev = newFrontIdx;
        m_data->next = newFrontIdx;
    }

    // Returns an element that the user should fill in before doing push_*
    T& node_obtain()
    {
        assert(m_free > 0);
        T& freeNode = m_data[m_free];
        m_free = freeNode.next;
        return freeNode;
    }

    bool empty()
    {
        return m_data->prev == 0;
    }

protected:
    T* m_data;
    index_t m_free;
    index_t m_capacity;
    static constexpr index_t SENTINEL_IDX = 0;
};

}
