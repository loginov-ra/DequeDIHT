#pragma once
#include <iterator>

const int INF = 1000000000;

template <class Type, class Pointer, class Reference>
class DequeIterator : public std::iterator<std::random_access_iterator_tag, Type, std::ptrdiff_t, Pointer, Reference>
{
private:
    const int* headInd_;
    Pointer elem_;
    const size_t* dequeCapacity_;
    const size_t* dequeSize_;
    Type* buffer_;

    int getRealIndex() const
    {
        if (elem_ < buffer_)
            return -1;

        if (elem_ >= buffer_ + *dequeCapacity_)
            return INF;

        Pointer head_ = buffer_ + *headInd_;
        return (head_ > elem_) ? (*dequeCapacity_ - (head_ - elem_)) : (elem_ - head_);
    }

    Pointer getPointerByIndex(int index) const
    {
        int realIndex = (*headInd_ + index) % *dequeCapacity_;
        return buffer_ + realIndex;
    }
public:
    DequeIterator(const int* headInd, Pointer elem, const size_t* dequeCapacity, const size_t* dequeSize, Type* buffer):
        headInd_(headInd),
        elem_(elem),
        dequeCapacity_(dequeCapacity),
        dequeSize_(dequeSize),
        buffer_(buffer)

        {}

    DequeIterator& operator +=(int n)
    {
        int myIndex = getRealIndex();
        int newIndex = myIndex + n;

        if (newIndex < 0)
        {
            elem_ = buffer_ - 1;
            return *this;
        }

        if (newIndex > static_cast<int>(*dequeSize_))
        {
            elem_ = buffer_ + *dequeCapacity_;
            return *this;
        }

        Pointer newElem = getPointerByIndex(myIndex + n);
        elem_ = newElem;
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