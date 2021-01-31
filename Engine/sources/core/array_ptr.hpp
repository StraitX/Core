#ifndef STRAITX_ARRAY_PTR_HPP
#define STRAITX_ARRAY_PTR_HPP 

#include <initializer_list>
#include "platform/types.hpp"
#include "core/assert.hpp"
#include "core/push_array.hpp"

namespace StraitX{

// it has no idea about allocations
template <typename T_Type, typename T_Size = size_t>
struct ArrayPtr{
    T_Type *const Pointer;
    const T_Size Size;

    typedef T_Type * iterator;
    typedef const T_Type * const_iterator;

    constexpr ArrayPtr();

    constexpr ArrayPtr(T_Type *pointer, T_Size size);

    constexpr ArrayPtr(const ArrayPtr &other);

    constexpr ArrayPtr(std::initializer_list<T_Type> initializer_list);

    template <size_t T_Capacity>
    constexpr ArrayPtr(PushArray<T_Type,T_Capacity> &push_array);

    constexpr T_Type &operator[](T_Size index);

    constexpr const T_Type &operator[](T_Size index)const;

    constexpr ArrayPtr &operator=(const ArrayPtr &other);
    
    constexpr iterator begin();

    constexpr iterator end();

    constexpr const_iterator begin()const;

    constexpr const_iterator end()const;

};

template <typename T_Type, typename T_Size>
constexpr ArrayPtr<T_Type, T_Size>::ArrayPtr():
    Pointer(nullptr),
    Size(0)
{}

template <typename T_Type, typename T_Size>
constexpr ArrayPtr<T_Type, T_Size>::ArrayPtr(T_Type *pointer, T_Size size):
    Pointer(pointer),
    Size(size)
{}

template <typename T_Type, typename T_Size>
constexpr ArrayPtr<T_Type, T_Size>::ArrayPtr(const ArrayPtr &other):
    Pointer(other.Pointer),
    Size(other.Size)
{}

template <typename T_Type, typename T_Size>
constexpr ArrayPtr<T_Type, T_Size>::ArrayPtr(std::initializer_list<T_Type> initializer_list):
    Pointer(initializer_list.begin()),
    Size(initializer_list.size())
{}

template <typename T_Type, typename T_Size>
template <size_t T_Capacity>
constexpr ArrayPtr<T_Type, T_Size>::ArrayPtr(PushArray<T_Type,T_Capacity> &push_array):
    Pointer(push_array.begin()),
    Size(push_array.Size())
{}

template <typename T_Type, typename T_Size>
constexpr T_Type &ArrayPtr<T_Type, T_Size>::operator[](T_Size index){
    CoreAssert(index < Size, "ArrayPtr: can't index more that ArrayPtr::Size elements");
    return Pointer[index]; 
}

template <typename T_Type, typename T_Size>
constexpr const T_Type &ArrayPtr<T_Type, T_Size>::operator[](T_Size index)const{
    return operator[](index);
}

template <typename T_Type, typename T_Size>
constexpr ArrayPtr<T_Type,T_Size> &ArrayPtr<T_Type, T_Size>::operator=(const ArrayPtr &other){
    Pointer = other.Pointer;
    Size = other.Size;
}

template <typename T_Type, typename T_Size>
constexpr typename ArrayPtr<T_Type, T_Size>::iterator ArrayPtr<T_Type, T_Size>::begin(){
    return Pointer;
}

template <typename T_Type, typename T_Size>
constexpr typename ArrayPtr<T_Type, T_Size>::iterator ArrayPtr<T_Type, T_Size>::end(){
    return Pointer+Size;
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