#include <cstddef>
#include <cassert>

namespace eds
{

template <typename T>
class LinkedListIterator
{
public:
    LinkedListIterator(T* data, size_t index) :
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

    bool operator==(const LinkedListIterator<T>& rhs)
    {
        return rhs.m_data == m_data &&
            rhs.m_index == m_index;
    }

    bool operator!=(const LinkedListIterator<T>& rhs)
    {
        return !operator==(rhs);
    }

    LinkedListIterator<T> operator++(int) // postfix
    {
        LinkedListIterator<T> temp = *this;
        ++(*this);
        return temp;
    }


private:
    T* m_data;
    size_t m_index;
};

template <typename T>
class LinkedList
{
public:
    LinkedList(size_t capacity) :
        m_data(new T[capacity]),
        m_free{1}
    {
        m_data = new T[capacity+1];

        // set up the free list
        for (size_t i = 1; i < capacity+1; i++)
        {
            m_data[i].next = (i+1) % (capacity+1);
        }
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
        size_t head = m_data->next;
        return m_data[head];
    }

    T& back()
    {
        size_t tail = m_data->prev;
        return m_data[tail];
    }

    LinkedListIterator<T> begin()
    {
        return LinkedListIterator<T>(m_data, m_data->next);
    }

    LinkedListIterator<T> end()
    {
        return LinkedListIterator<T>(m_data, SENTINEL_IDX);
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
        T& back = m_data[m_data->prev];

        m_data->prev = back.prev;
        T& newBack = m_data[back.prev];
        newBack.next = SENTINEL_IDX;
    }

    T& pop_front()
    {
        // TODO: consider using remove() instead of code duplication here?
        // Will this improve the i-cache performance?
        size_t frontIdx = m_data->next;
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
        size_t newFrontIdx = (&item - m_data);
        size_t oldFrontIdx = m_data->next;

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

private:
    T* m_data;
    size_t m_free;
    size_t m_capacity;
    static constexpr size_t SENTINEL_IDX = 0;
};

}
