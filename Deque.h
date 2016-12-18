#pragma once
#include <iostream>
#include <cassert>
#include <cmath>
#include "DequeIterator.h"

const size_t START_CAPACITY = 4;
const int CAPACITY_MULTIPLYER = 2;

template <class Type>
class Deque
{
private:
    typedef DequeIterator<Type, Type*, Type&> iterator;
    typedef DequeIterator<Type, const Type*, const Type&> const_iterator;
    typedef std::reverse_iterator<DequeIterator<Type, Type*, Type&>> reverse_iterator;
    typedef std::reverse_iterator<DequeIterator<Type, const Type*, const Type&>> const_reverse_iterator;

    Type* buffer_;
    size_t size_;
    size_t capacity_;
    int head_;
    int tail_; //Tail is not a part of a queue

    void reallocateBuffer(size_t new_size);
    void extendBuffer();
    void narrowBuffer();

    int indexInBuffer(int i) const;
public:
    Deque():
        buffer_(nullptr),
        size_(0),
        capacity_(START_CAPACITY),
        head_(0),
        tail_(0)
    {
        buffer_ = new Type[START_CAPACITY];
    }

    Deque(const Deque& that):
        buffer_(nullptr),
        size_(that.size_),
        capacity_(that.capacity_),
        head_(that.head_),
        tail_(that.tail_)
    {
        buffer_ = new Type[capacity_];
        std::copy(that.buffer_, that.buffer_ + that.capacity_, buffer_);
    }

    ~Deque()
    {
        delete[] buffer_;
    }

    bool empty() const;
    size_t size() const;

    void push_back(const Type& elem);
    void push_front(const Type& elem);
    void pop_back();
    void pop_front();

    Type& operator[](int i);
    const Type& operator[](int i) const;

    const Type& back() const;
    const Type& front() const;
    Type& back();
    Type& front();

    iterator begin();
    const_iterator begin() const;
    const_iterator cbegin() const;

    iterator end();
    const_iterator end() const;
    const_iterator cend() const;

    reverse_iterator rbegin();
    const_reverse_iterator rbegin() const;
    const_reverse_iterator crbegin() const;

    reverse_iterator rend();
    const_reverse_iterator rend() const;
    const_reverse_iterator crend() const;
};

template <class Type>
bool Deque<Type>::empty() const
{
    return (size_ == 0);
}

template <class Type>
size_t Deque<Type>::size() const
{
    return size_;
}

template <class Type>
void Deque<Type>::reallocateBuffer(size_t new_capacity)
{
    if (new_capacity < CAPACITY_MULTIPLYER * CAPACITY_MULTIPLYER)
        return;

    Type* bufferCopy = new Type[capacity_];
    std::copy(buffer_, buffer_ + capacity_, bufferCopy);
    delete[] buffer_;
    buffer_ = new Type[new_capacity];

    for (size_t i = 0; i < size_; ++i)
        buffer_[i] = buffer_copy[(head_ + i) % capacity_]; 

    head_ = 0;
    tail_ = size_;
    capacity_ = new_capacity;
}

template <class Type>
void Deque<Type>::extendBuffer()
{
    if (head_ == static_cast<int>((tail_ + 1) % capacity_))
        reallocateBuffer(capacity_ * CAPACITY_MULTIPLYER);
}

template <class Type>
void Deque<Type>::narrowBuffer()
{
    if (size_ * CAPACITY_MULTIPLYER * CAPACITY_MULTIPLYER <= capacity_)
        reallocateBuffer(capacity_ / CAPACITY_MULTIPLYER);
}

template <class Type>
void Deque<Type>::push_back(const Type& elem)
{
    buffer_[tail_] = elem;
    tail_ = (tail_ + 1) % capacity_;
    ++size_;
    extendBuffer();
}

template <class Type>
void Deque<Type>::push_front(const Type& elem)
{
    head_ = (head_ + capacity_ - 1) % capacity_;
    buffer_[head_] = elem;
    ++size_;
    extendBuffer();
}

template <class Type>
void Deque<Type>::pop_back()
{
    assert(!empty());
    tail_ = (tail_ + capacity_ - 1) % capacity_;
    size_--;
    narrowBuffer();
}

template <class Type>
void Deque<Type>::pop_front()
{
    assert(!empty());
    head_ = (head_ + 1) % capacity_;
    size_--;
    narrowBuffer();
}

template <class Type>
int Deque<Type>::indexInBuffer(int i) const
{
    assert(i >= 0);
    assert(i < static_cast<int>(size_));

    return (head_ + i) % capacity_;
}

template <class Type>
Type& Deque<Type>::operator [](int i)
{
    return buffer_[indexInBuffer(i)];
}

template <class Type>
const Type& Deque<Type>::operator [](int i) const
{
    return buffer_[indexInBuffer(i)];
}

template <class Type>
Type& Deque<Type>::back()
{
    return buffer_[(tail_ + capacity_ - 1) % capacity_];
}

template <class Type>
const Type& Deque<Type>::back() const
{
    return buffer_[(tail_ + capacity_ - 1) % capacity_];
}

template <class Type>
Type& Deque<Type>::front()
{
    return buffer_[head_];
}

template <class Type>
const Type& Deque<Type>::front() const
{
    return buffer_[head_];
}

template <class Type>
typename Deque<Type>::iterator Deque<Type>::begin()
{
    return iterator(&head_, &buffer_[head_], &capacity_, &size_, buffer_);
}

template <class Type>
typename Deque<Type>::const_iterator Deque<Type>::cbegin() const
{
    return const_iterator(&head_, buffer_ + head_, &capacity_, &size_, buffer_); 
}

template <class Type>
typename Deque<Type>::const_iterator Deque<Type>::begin() const
{
    return cbegin(); 
}

template <class Type>
typename Deque<Type>::iterator Deque<Type>::end()
{
    return iterator(&head_, &buffer_[tail_], &capacity_, &size_, buffer_); 
}

template <class Type>
typename Deque<Type>::const_iterator Deque<Type>::cend() const
{
    return const_iterator(&head_, &buffer_[tail_], &capacity_, &size_, buffer_); 
}

template <class Type>
typename Deque<Type>::const_iterator Deque<Type>::end() const
{
    return const_iterator(&head_, &buffer_[tail_], &capacity_, &size_, buffer_); 
}

template <class Type>
typename Deque<Type>::reverse_iterator Deque<Type>::rbegin()
{
    return reverse_iterator(end());
}

template <class Type>
typename Deque<Type>::reverse_iterator Deque<Type>::rend()
{
    return reverse_iterator(begin());
}

template <class Type>
typename Deque<Type>::const_reverse_iterator Deque<Type>::crbegin() const
{
    return const_reverse_iterator(cend());
}

template <class Type>
typename Deque<Type>::const_reverse_iterator Deque<Type>::crend() const
{
    return const_reverse_iterator(cbegin());
}

template <class Type>
typename Deque<Type>::const_reverse_iterator Deque<Type>::rend() const
{
    return crend();
}

template <class Type>
typename Deque<Type>::const_reverse_iterator Deque<Type>::rbegin() const
{
    return crbegin();
}