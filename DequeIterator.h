#pragma once
#include <iterator>

const int INF = 1000000000;

template <class Type, class Pointer, class Reference>
class DequeIterator : public std::iterator<std::random_access_iterator_tag, Type, std::ptrdiff_t, Pointer, Reference>
{
private:
    const int* head_ind_;
    Pointer elem_;
    const size_t* deque_capacity_;
    const size_t* deque_size_;
    Type* buffer_;

    int getRealIndex() const
    {
        if (elem_ < buffer_)
            return -1;

        if (elem_ >= buffer_ + *deque_capacity_)
            return INF;

        Pointer head_ = buffer_ + *head_ind_;
        return (head_ > elem_) ? (*deque_capacity_ - (head_ - elem_)) : (elem_ - head_);
    }

    Pointer getPointerByIndex(int index) const
    {
        int real_index = (*head_ind_ + index) % *deque_capacity_;
        return buffer_ + real_index;
    }
public:
    DequeIterator(const int* head_ind, Pointer elem, const size_t* deque_capacity, const size_t* deque_size, Type* buffer):
        head_ind_(head_ind),
        elem_(elem),
        deque_capacity_(deque_capacity),
        deque_size_(deque_size),
        buffer_(buffer)

        {}

    DequeIterator& operator +=(int n)
    {
        int my_index = getRealIndex();
        int new_index = my_index + n;

        if (new_index < 0)
        {
            elem_ = buffer_ - 1;
            return *this;
        }

        if (new_index > *deque_size_)
        {
            elem_ = buffer_ + *deque_capacity_;
            return *this;
        }

        Pointer new_elem = getPointerByIndex(my_index + n);
        elem_ = new_elem;
        return *this;
    }

    DequeIterator operator +(int n) const
    {
        DequeIterator result = *this;
        result += n;
        return result;
    }

    DequeIterator& operator -=(int n)
    {
        return *this += (-n);
    }
 
    DequeIterator operator -(int n) const
    {
        return *this + (-n);
    }

    DequeIterator& operator ++()
    {
        return *this += 1;
    }

    DequeIterator& operator --()
    {
        return *this -= 1;
    }

    DequeIterator operator ++(int)
    {
        DequeIterator res = *this;
        *this += 1;
        return res;
    }

    DequeIterator operator --(int)
    {
        DequeIterator res = *this;
        *this -= 1;
        return res;
    }

    int operator -(const DequeIterator& that) const
    {
        return getRealIndex() - that.getRealIndex();
    }

    Reference operator *() const
    {
        return *elem_;
    }

    Reference operator [](int n) const
    {
        return *(*this + n);
    }

    Pointer operator ->() const
    {
        return elem_;
    }

    bool operator ==(const DequeIterator& that) const
    {
        return (that.elem_ == elem_);
    }

    bool operator <(const DequeIterator& that) const
    {
        return (getRealIndex() < that.getRealIndex());
    }

    bool operator >(const DequeIterator& that) const
    {
        return that < *this;
    }

    bool operator <=(const DequeIterator& that) const
    {
        return !(*this > that);
    }

    bool operator >=(const DequeIterator& that) const
    {
        return !(that > *this);
    }

    bool operator !=(const DequeIterator& that) const
    {
        return !(*this == that);
    }
};

template <class Type, class Pointer, class Reference>
DequeIterator<Type, Pointer, Reference> operator +(DequeIterator<Type, Pointer, Reference> iter, int n)
{
    return iter + n;
}