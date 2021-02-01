#ifndef STRAITX_ARRAY_PTR_HPP
#define STRAITX_ARRAY_PTR_HPP 

#include <initializer_list>
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

    constexpr ArrayPtr(const ArrayPtr &other);

    constexpr ArrayPtr(std::initializer_list<T_Type> initializer_list);

    template <size_t T_Capacity>
    constexpr ArrayPtr(PushArray<T_Type,T_Capacity> &push_array);

    constexpr T_Type &operator[](T_Size index);

    constexpr const T_Type &operator[](T_Size index)const;

    constexpr ArrayPtr &operator=(const ArrayPtr &other);

    constexpr T_Size Size()const;

    constexpr T_Type *Data();

    constexpr const T_Type *Data()const;
    
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
constexpr ArrayPtr<T_Type, T_Size>::ArrayPtr(const ArrayPtr &other):
    m_Pointer(other.m_Pointer),
    m_Size(other.Size)
{}

template <typename T_Type, typename T_Size>
constexpr ArrayPtr<T_Type, T_Size>::ArrayPtr(std::initializer_list<T_Type> initializer_list):
    m_Pointer(initializer_list.begin()),
    m_Size(initializer_list.size())
{
    static_assert(IsConst<T_Type>::Value, "ArrayPtr with non-const type can't be constructed via initializer list");
}

template <typename T_Type, typename T_Size>
template <size_t T_Capacity>
constexpr ArrayPtr<T_Type, T_Size>::ArrayPtr(PushArray<T_Type,T_Capacity> &push_array):
    m_Pointer(push_array.begin()),
    m_Size(push_array.Size())
{}

template <typename T_Type, typename T_Size>
constexpr T_Type &ArrayPtr<T_Type, T_Size>::operator[](T_Size index){
    CoreAssert(index < m_Size, "ArrayPtr: can't index more that ArrayPtr::Size elements");
    return m_Pointer[index]; 
}

template <typename T_Type, typename T_Size>
constexpr const T_Type &ArrayPtr<T_Type, T_Size>::operator[](T_Size index)const{
    return operator[](index);
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
constexpr T_Type *ArrayPtr<T_Type, T_Size>::Data(){
    return m_Pointer;
}

template <typename T_Type, typename T_Size>
constexpr const T_Type *ArrayPtr<T_Type, T_Size>::Data()const{
    return Data();
}

template <typename T_Type, typename T_Size>
constexpr typename ArrayPtr<T_Type, T_Size>::iterator ArrayPtr<T_Type, T_Size>::begin(){
    return Data();
}

template <typename T_Type, typename T_Size>
constexpr typename ArrayPtr<T_Type, T_Size>::iterator ArrayPtr<T_Type, T_Size>::end(){
    return Data()+Size();
}

template <typename T_Type, typename T_Size>
constexpr typename ArrayPtr<T_Type, T_Size>::const_iterator ArrayPtr<T_Type, T_Size>::begin()const{
    return begin();
}

template <typename T_Type, typename T_Size>
constexpr typename ArrayPtr<T_Type, T_Size>::const_iterator ArrayPtr<T_Type, T_Size>::end()const{
    return end();
}

}; // namespace StraitX::

#endif //STRAITX_ARRAY_PTR_HPP