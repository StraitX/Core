#ifndef STRAITX_PUSH_ARRAY_HPP
#define STRAITX_PUSH_ARRAY_HPP

#include <new>
#include <initializer_list>
#include "platform/types.hpp"
#include "core/assert.hpp"
#include "core/move.hpp"

namespace StraitX{

template<typename T_Type, size_t T_Capacity>
class PushArray{
public:
    typedef T_Type * iterator;
    typedef const T_Type * const_iterator;
private:
    T_Type m_Array[T_Capacity] = {};
    size_t m_Size = 0;
public:
    constexpr PushArray() = default;

    PushArray(const PushArray &other) = delete;

    PushArray(PushArray &&other) = delete;

    constexpr PushArray(std::initializer_list<T_Type> initializer_list);

    constexpr void Push(const T_Type &element);

    constexpr void Push(T_Type &&element);

    constexpr void Pop();

    template<typename ...T_Args>
    constexpr void Emplace(T_Args&&...args);

    constexpr T_Type &operator[](size_t index);

    constexpr const T_Type &operator[](size_t index)const;

    constexpr size_t Size()const;

    constexpr size_t Capacity()const;

    constexpr iterator begin();

    constexpr iterator end();

    constexpr const_iterator begin()const;

    constexpr const_iterator end()const;
};


template<typename T_Type, size_t T_Capacity>
constexpr PushArray<T_Type, T_Capacity>::PushArray(std::initializer_list<T_Type> initializer_list){
    CoreAssert(initializer_list.size() <= T_Capacity, "Initializer list is bigger than PushArray capacity");

    size_t i = 0;
    for(auto &e: initializer_list){
        m_Array[i] = e;
        ++i;
    }

    m_Size = initializer_list.size();
}

template<typename T_Type, size_t T_Capacity>
constexpr void PushArray<T_Type, T_Capacity>::Push(const T_Type &element){
    CoreAssert(m_Size < T_Capacity, "PushArray: Can't push an element, array is full");
    m_Array[m_Size++] = element;
}

template<typename T_Type, size_t T_Capacity>
constexpr void PushArray<T_Type, T_Capacity>::Push(T_Type &&element){
    CoreAssert(m_Size < T_Capacity, "PushArray: Can't push an element, array is full");
    m_Array[m_Size++] = Move(element);
}

template<typename T_Type, size_t T_Capacity>
constexpr void PushArray<T_Type, T_Capacity>::Pop(){
    m_Array[--m_Size].~T_Type();
}

template<typename T_Type, size_t T_Capacity>
template<typename ...T_Args>
constexpr void PushArray<T_Type, T_Capacity>::Emplace(T_Args&&...args){
    CoreAssert(m_Size < T_Capacity, "PushArray: Can't emplace an element, array is full");
    new(&m_Array[m_Size++])T_Type(args...);
}

template<typename T_Type, size_t T_Capacity>
constexpr T_Type &PushArray<T_Type, T_Capacity>::operator[](size_t index){
    CoreAssert(index < m_Size, "PushArray: Can't index more than PushArray::Size() elements");
    return m_Array[index];
}

template<typename T_Type, size_t T_Capacity>
constexpr const T_Type &PushArray<T_Type, T_Capacity>::operator[](size_t index)const{
    return operator[](index);
}

template<typename T_Type, size_t T_Capacity>
constexpr size_t PushArray<T_Type, T_Capacity>::Size()const{
    return m_Size;
}

template<typename T_Type, size_t T_Capacity>
constexpr size_t PushArray<T_Type, T_Capacity>::Capacity()const{
    return T_Capacity;
}

template<typename T_Type, size_t T_Capacity>
constexpr typename PushArray<T_Type, T_Capacity>::iterator PushArray<T_Type, T_Capacity>::begin(){
    return m_Array;
}

template<typename T_Type, size_t T_Capacity>
constexpr typename PushArray<T_Type, T_Capacity>::iterator PushArray<T_Type, T_Capacity>::end(){
    return begin()+Size();
}

template<typename T_Type, size_t T_Capacity>
constexpr typename PushArray<T_Type, T_Capacity>::const_iterator PushArray<T_Type, T_Capacity>::begin()const{
    return begin();
}

template<typename T_Type, size_t T_Capacity>
constexpr typename PushArray<T_Type, T_Capacity>::const_iterator PushArray<T_Type, T_Capacity>::end()const{
    return end();
}

}//namespace StraitX::

#endif//STRAITX_PUSH_ARRAY_HPP