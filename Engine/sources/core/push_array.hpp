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
    // we don't want c++ to construct objects for us
    u8 m_Memory[T_Capacity * sizeof(T_Type)];
    size_t m_Size = 0;
public:
    constexpr PushArray() = default;

    constexpr PushArray(const PushArray &other) = default;

    constexpr PushArray(std::initializer_list<T_Type> initializer_list);

    ~PushArray();

    constexpr void Push(const T_Type &element);

    constexpr void Push(T_Type &&element);

    constexpr void Pop();

    constexpr void Clear();

    template<typename ...T_Args>
    constexpr void Emplace(T_Args&&...args);

    constexpr T_Type &operator[](size_t index);

    constexpr const T_Type &operator[](size_t index)const;

    constexpr PushArray &operator=(const PushArray &other);

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
        operator[](i) = e;
        ++i;
    }

    m_Size = initializer_list.size();
}

template<typename T_Type, size_t T_Capacity>
PushArray<T_Type, T_Capacity>::~PushArray(){
    Clear();
}

template<typename T_Type, size_t T_Capacity>
constexpr void PushArray<T_Type, T_Capacity>::Push(const T_Type &element){
    CoreAssert(m_Size < T_Capacity, "PushArray: Can't push an element, array is full");
    operator[](m_Size++) = element;
}

template<typename T_Type, size_t T_Capacity>
constexpr void PushArray<T_Type, T_Capacity>::Push(T_Type &&element){
    CoreAssert(m_Size < T_Capacity, "PushArray: Can't push an element, array is full");
    operator[](m_Size++) = Move(element);
}

template<typename T_Type, size_t T_Capacity>
constexpr void PushArray<T_Type, T_Capacity>::Pop(){
    begin()[--m_Size].~T_Type();
}

template<typename T_Type, size_t T_Capacity>
constexpr void PushArray<T_Type, T_Capacity>::Clear(){
    while(Size())
        Pop();
}

template<typename T_Type, size_t T_Capacity>
template<typename ...T_Args>
constexpr void PushArray<T_Type, T_Capacity>::Emplace(T_Args&&...args){
    CoreAssert(m_Size < T_Capacity, "PushArray: Can't emplace an element, array is full");
    new(&operator[](m_Size++))T_Type(args...);
}

template<typename T_Type, size_t T_Capacity>
constexpr T_Type &PushArray<T_Type, T_Capacity>::operator[](size_t index){
    CoreAssert(index < m_Size, "PushArray: Can't index more than PushArray::Size() elements");
    return begin()[index];
}

template<typename T_Type, size_t T_Capacity>
constexpr const T_Type &PushArray<T_Type, T_Capacity>::operator[](size_t index)const{
    CoreAssert(index < m_Size, "PushArray: Can't index more than PushArray::Size() elements");
    return begin()[index];
}

template<typename T_Type, size_t T_Capacity>
constexpr PushArray<T_Type, T_Capacity> &PushArray<T_Type, T_Capacity>::operator=(const PushArray &other){
    for(size_t i = 0; i<other.m_Size; ++i)
        operator[](i) = other.m_Array[i];
    m_Size = other.m_Size;
    return *this;
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
    return reinterpret_cast<T_Type*>(m_Memory);
}

template<typename T_Type, size_t T_Capacity>
constexpr typename PushArray<T_Type, T_Capacity>::iterator PushArray<T_Type, T_Capacity>::end(){
    return begin()+Size();
}

template<typename T_Type, size_t T_Capacity>
constexpr typename PushArray<T_Type, T_Capacity>::const_iterator PushArray<T_Type, T_Capacity>::begin()const{
    return reinterpret_cast<const T_Type*>(m_Memory);
}

template<typename T_Type, size_t T_Capacity>
constexpr typename PushArray<T_Type, T_Capacity>::const_iterator PushArray<T_Type, T_Capacity>::end()const{
    return begin()+Size();
}

}//namespace StraitX::

#endif//STRAITX_PUSH_ARRAY_HPP