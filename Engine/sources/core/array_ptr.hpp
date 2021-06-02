#ifndef STRAITX_ARRAY_PTR_HPP
#define STRAITX_ARRAY_PTR_HPP 

#include "platform/types.hpp"
#include "core/assert.hpp"
#include "core/push_array.hpp"

namespace StraitX{

// it has no idea about allocations, object lifetime and stuff
template <typename T_Type, typename T_Size = size_t>
class ArrayPtr{
private:
    T_Type *m_Pointer = nullptr;
    T_Size m_Size = 0;
public:

    constexpr ArrayPtr() = default;

    constexpr ArrayPtr(T_Type *pointer, T_Size size);

    template<size_t T_ArraySize>
    constexpr ArrayPtr(T_Type (&array)[T_ArraySize]);

    constexpr ArrayPtr(const ArrayPtr &other);

    template <size_t T_Capacity>
    constexpr ArrayPtr(const PushArray<T_Type,T_Capacity> &push_array);

    constexpr T_Type &operator[](T_Size index);

    constexpr const T_Type &operator[](T_Size index)const;

    constexpr ArrayPtr &operator=(const ArrayPtr &other);

    constexpr T_Size Size()const;

    constexpr T_Type *Pointer();

    constexpr const T_Type *Pointer()const;
    
    typedef T_Type* iterator;
    typedef const T_Type* const_iterator;

    constexpr iterator begin();

    constexpr iterator end();

    constexpr const_iterator begin()const;

    constexpr const_iterator end()const;

};

template <typename T_Type, typename T_Size>
constexpr ArrayPtr<T_Type, T_Size>::ArrayPtr(T_Type *pointer, T_Size size):
    m_Pointer(pointer),
    m_Size(size)
{}


template <typename T_Type, typename T_Size>
template<size_t T_ArraySize>
constexpr ArrayPtr<T_Type, T_Size>::ArrayPtr(T_Type (&array)[T_ArraySize]):
    m_Pointer(&array[0]),
    m_Size(T_ArraySize)
{}

template <typename T_Type, typename T_Size>
constexpr ArrayPtr<T_Type, T_Size>::ArrayPtr(const ArrayPtr &other):
    m_Pointer(other.m_Pointer),
    m_Size(other.m_Size)
{}

template <typename T_Type, typename T_Size>
template <size_t T_Capacity>
constexpr ArrayPtr<T_Type, T_Size>::ArrayPtr(const PushArray<T_Type,T_Capacity> &push_array):
    m_Pointer(const_cast<iterator>(push_array.begin())),
    m_Size(push_array.Size())
{}

template <typename T_Type, typename T_Size>
constexpr T_Type &ArrayPtr<T_Type, T_Size>::operator[](T_Size index){
    SX_CORE_ASSERT(index < m_Size, "ArrayPtr: can't index more that ArrayPtr::Size elements");
    return m_Pointer[index]; 
}

template <typename T_Type, typename T_Size>
constexpr const T_Type &ArrayPtr<T_Type, T_Size>::operator[](T_Size index)const{
    SX_CORE_ASSERT(index < m_Size, "ArrayPtr: can't index more that ArrayPtr::Size elements");
    return m_Pointer[index]; 
}

template <typename T_Type, typename T_Size>
constexpr ArrayPtr<T_Type,T_Size> &ArrayPtr<T_Type, T_Size>::operator=(const ArrayPtr &other){
    m_Pointer = other.m_Pointer;
    m_Size = other.m_Size;
    return *this;
}

template <typename T_Type, typename T_Size>
constexpr T_Size ArrayPtr<T_Type, T_Size>::Size()const{
    return m_Size;
}

template <typename T_Type, typename T_Size>
constexpr T_Type *ArrayPtr<T_Type, T_Size>::Pointer(){
    return m_Pointer;
}

template <typename T_Type, typename T_Size>
constexpr const T_Type *ArrayPtr<T_Type, T_Size>::Pointer()const{
    return m_Pointer;
}

template <typename T_Type, typename T_Size>
constexpr typename ArrayPtr<T_Type, T_Size>::iterator ArrayPtr<T_Type, T_Size>::begin(){
    return Pointer();
}

template <typename T_Type, typename T_Size>
constexpr typename ArrayPtr<T_Type, T_Size>::iterator ArrayPtr<T_Type, T_Size>::end(){
    return Pointer()+Size();
}

template <typename T_Type, typename T_Size>
constexpr typename ArrayPtr<T_Type, T_Size>::const_iterator ArrayPtr<T_Type, T_Size>::begin()const{
    return Pointer();
}

template <typename T_Type, typename T_Size>
constexpr typename ArrayPtr<T_Type, T_Size>::const_iterator ArrayPtr<T_Type, T_Size>::end()const{
    return Pointer()+Size();
}

}; // namespace StraitX::

#endif //STRAITX_ARRAY_PTR_HPP