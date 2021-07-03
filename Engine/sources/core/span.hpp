#ifndef STRAITX_SPAN_HPP
#define STRAITX_SPAN_HPP 

#include "platform/types.hpp"
#include "core/assert.hpp"

// it has no idea about allocations, object lifetime and stuff
template <typename T_Type, typename T_Size = size_t>
class Span{
private:
    T_Type *m_Pointer = nullptr;
    T_Size m_Size = 0;
public:

    Span() = default;

    Span(T_Type *pointer, T_Size size);

    template<size_t T_ArraySize>
    Span(T_Type (&array)[T_ArraySize]);

    Span(const Span &other);

    T_Type &operator[](T_Size index)const;

    Span &operator=(const Span &other);

    T_Size Size()const;

    T_Type *Pointer()const;
    
    typedef T_Type* iterator;

    iterator begin()const;

    iterator end()const;
};

template <typename T_Type, typename T_Size>
Span<T_Type, T_Size>::Span(T_Type *pointer, T_Size size):
    m_Pointer(pointer),
    m_Size(size)
{}


template <typename T_Type, typename T_Size>
template<size_t T_ArraySize>
Span<T_Type, T_Size>::Span(T_Type (&array)[T_ArraySize]):
    m_Pointer(&array[0]),
    m_Size(T_ArraySize)
{}

template <typename T_Type, typename T_Size>
Span<T_Type, T_Size>::Span(const Span &other):
    m_Pointer(other.m_Pointer),
    m_Size(other.m_Size)
{}

template <typename T_Type, typename T_Size>
T_Type &Span<T_Type, T_Size>::operator[](T_Size index)const{
    SX_CORE_ASSERT(index < m_Size, "Span: can't index more that Span::Size elements");
    return m_Pointer[index]; 
}

template <typename T_Type, typename T_Size>
Span<T_Type,T_Size> &Span<T_Type, T_Size>::operator=(const Span &other){
    m_Pointer = other.m_Pointer;
    m_Size = other.m_Size;
    return *this;
}

template <typename T_Type, typename T_Size>
T_Size Span<T_Type, T_Size>::Size()const{
    return m_Size;
}

template <typename T_Type, typename T_Size>
T_Type *Span<T_Type, T_Size>::Pointer()const{
    return m_Pointer;
}

template <typename T_Type, typename T_Size>
typename Span<T_Type, T_Size>::iterator Span<T_Type, T_Size>::begin()const{
    return Pointer();
}

template <typename T_Type, typename T_Size>
typename Span<T_Type, T_Size>::iterator Span<T_Type, T_Size>::end()const{
    return Pointer()+Size();
}

#endif //STRAITX_SPAN_HPP