# DequeDIHT
Realization of deque with all types of random access iterators

Repository consists of two filed now:

1) DequeIterator.h
    This is where template class of iterator is implemented. Template parameters are practically the same to STL
    Neccessary operators are overloaded. Iterators stay valid until reallocation happens
    I want to point out that if your iterator became non-valid or not dereferenceable it mustn't work correctly. Be careful
    
2) Deque.h
    Template class Deque is implemented there. All operations required by abstract data type Deque are done including operator [] of random access to queue elements. Iterator, const_itetator, reverse_iterator and const_reverse_iterator are implemented too.
    
3) Testing. It is in progress.
