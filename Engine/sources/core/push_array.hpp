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
    size_t m_Size = 0;
    u8 m_Memory[T_Capacity * sizeof(T_Type)];
public:
    constexpr PushArray() = default;

    constexpr PushArray(const PushArray &other);

    constexpr PushArray(PushArray &&other);

    constexpr PushArray(std::initializer_list<T_Type> initializer_list);

    ~PushArray();

    constexpr void Push(const T_Type &element);

    constexpr void Push(T_Type &&element);

    constexpr void Pop();

    constexpr void Clear();

    iterator Find(const T_Type &element);

    const_iterator Find(const T_Type &element)const;

    template<typename ...T_Args>
    constexpr T_Type &Emplace(T_Args&&...args);

    constexpr T_Type &operator[](size_t index);

    constexpr const T_Type &operator[](size_t index)const;

    constexpr PushArray &operator=(const PushArray &other);

    constexpr PushArray &operator=(PushArray &&other);

    constexpr size_t Size()const;

    constexpr size_t Capacity()const;

    constexpr iterator begin();

    constexpr iterator end();

    constexpr const_iterator begin()const;

    constexpr const_iterator end()const;
};

template<typename T_Type, size_t T_Capacity>
constexpr PushArray<T_Type, T_Capacity>::PushArray(const PushArray &other){
    *this = other;
}

template<typename T_Type, size_t T_Capacity>
constexpr PushArray<T_Type, T_Capacity>::PushArray(PushArray &&other){
    *this = Move(other);
}

template<typename T_Type, size_t T_Capacity>
constexpr PushArray<T_Type, T_Capacity>::PushArray(std::initializer_list<T_Type> initializer_list):
    m_Size(initializer_list.size())
{
    CoreAssert(initializer_list.size() <= T_Capacity, "Initializer list is bigger than PushArray capacity");

    size_t i = 0;
    for(auto &e: initializer_list){
        operator[](i) = e;
        ++i;
    }
}

template<typename T_Type, size_t T_Capacity>
PushArray<T_Type, T_Capacity>::~PushArray(){
    Clear();
}

template<typename T_Type, size_t T_Capacity>
constexpr void PushArray<T_Type, T_Capacity>::Push(const T_Type &element){
    Emplace(element);
}

template<typename T_Type, size_t T_Capacity>
constexpr void PushArray<T_Type, T_Capacity>::Push(T_Type &&element){
    Emplace(Move(element));
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
typename PushArray<T_Type, T_Capacity>::iterator PushArray<T_Type, T_Capacity>::Find(const T_Type &element){
    return const_cast<iterator>(const_cast<const PushArray<T_Type, T_Capacity>*>(this)->Find(element));
}

template<typename T_Type, size_t T_Capacity>
typename PushArray<T_Type, T_Capacity>::const_iterator PushArray<T_Type, T_Capacity>::Find(const T_Type &element)const{
    auto it = begin();
    for(;it != end(); ++it){
        if(*it == element)
            return it;
    }
    return it;
}

template<typename T_Type, size_t T_Capacity>
template<typename ...T_Args>
constexpr T_Type &PushArray<T_Type, T_Capacity>::Emplace(T_Args&&...args){
    CoreAssert(m_Size < T_Capacity, "PushArray: Can't add an element, array is full");

    return *new(&begin()[m_Size++])T_Type(Forward<T_Args>(args)...);
}

template<typename T_Type, size_t T_Capacity>
constexpr T_Type &PushArray<T_Type, T_Capacity>::operator[](size_t index){
    return const_cast<T_Type &>(const_cast<const PushArray<T_Type, T_Capacity>*>(this)->operator[](index));
}

template<typename T_Type, size_t T_Capacity>
constexpr const T_Type &PushArray<T_Type, T_Capacity>::operator[](size_t index)const{
    CoreAssert(index < m_Size, "PushArray: Can't index more than PushArray::Size() elements");

    return begin()[index];
}

template<typename T_Type, size_t T_Capacity>
constexpr PushArray<T_Type, T_Capacity> &PushArray<T_Type, T_Capacity>::operator=(const PushArray &other){
    Clear();
    for(auto &e: other)
        Push(e);
    return *this;
}

template<typename T_Type, size_t T_Capacity>
constexpr PushArray<T_Type, T_Capacity> &PushArray<T_Type, T_Capacity>::operator=(PushArray &&other){
    Clear();
    for(auto &e: other)
        Emplace(Move(e)); // We assume that moved stuff does not required to be destroyed
    other.m_Size = 0;
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