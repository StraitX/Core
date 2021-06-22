#ifndef STRAITX_PUSH_ARRAY_HPP
#define STRAITX_PUSH_ARRAY_HPP

#include <new>
#include <initializer_list>
#include "platform/types.hpp"
#include "core/assert.hpp"
#include "core/move.hpp"
#include "core/span.hpp"

namespace StraitX{

template<typename T_Type, size_t T_Capacity>
class PushArray{
public:
    typedef T_Type * iterator;
    typedef const T_Type * const_iterator;
private:
    // we don't want c++ to construct objects for us
    size_t m_Size = 0;
	class alignas(T_Type){
	private:
    	u8 m_Memory[T_Capacity * sizeof(T_Type)];
	public:
		T_Type *Data(){
			return reinterpret_cast<T_Type*>(m_Memory);
		}

		const T_Type *Data()const{
			return reinterpret_cast<const T_Type*>(m_Memory);
		}
	}m_Array;
public:
    static_assert(!IsConst<T_Type>() && !IsVolatile<T_Type>(), "T_Type can't be cv-qualified");

    PushArray() = default;

    PushArray(const PushArray &other);

    PushArray(PushArray &&other);

    PushArray(std::initializer_list<T_Type> initializer_list);

    ~PushArray();

    void Push(const T_Type &element);

    void Push(T_Type &&element);

    void Pop();

    void Clear();

    iterator Find(const T_Type &element);

    const_iterator Find(const T_Type &element)const;

    template<typename ...T_Args>
    T_Type &Emplace(T_Args&&...args);

    T_Type &operator[](size_t index);

    const T_Type &operator[](size_t index)const;

    PushArray &operator=(const PushArray &other);

    PushArray &operator=(PushArray &&other);

    operator Span<T_Type>();

    size_t Size()const;

    size_t Capacity()const;

    T_Type *Data();

    const T_Type *Data()const;

    iterator begin();

    iterator end();

    const_iterator begin()const;

    const_iterator end()const;
};

template<typename T_Type, size_t T_Capacity>
PushArray<T_Type, T_Capacity>::PushArray(const PushArray &other){
    *this = other;
}

template<typename T_Type, size_t T_Capacity>
PushArray<T_Type, T_Capacity>::PushArray(PushArray &&other){
    *this = Move(other);
}

template<typename T_Type, size_t T_Capacity>
PushArray<T_Type, T_Capacity>::PushArray(std::initializer_list<T_Type> initializer_list):
    m_Size(initializer_list.size())
{
    SX_CORE_ASSERT(initializer_list.size() <= T_Capacity, "Initializer list is bigger than PushArray capacity");

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
void PushArray<T_Type, T_Capacity>::Push(const T_Type &element){
    Emplace(element);
}

template<typename T_Type, size_t T_Capacity>
void PushArray<T_Type, T_Capacity>::Push(T_Type &&element){
    Emplace(Move(element));
}

template<typename T_Type, size_t T_Capacity>
void PushArray<T_Type, T_Capacity>::Pop(){
    begin()[--m_Size].~T_Type();
}

template<typename T_Type, size_t T_Capacity>
void PushArray<T_Type, T_Capacity>::Clear(){
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
T_Type &PushArray<T_Type, T_Capacity>::Emplace(T_Args&&...args){
    SX_CORE_ASSERT(m_Size < T_Capacity, "PushArray: Can't add an element, array is full");

    return *new(&begin()[m_Size++])T_Type(Forward<T_Args>(args)...);
}

template<typename T_Type, size_t T_Capacity>
T_Type &PushArray<T_Type, T_Capacity>::operator[](size_t index){
    return const_cast<T_Type &>(const_cast<const PushArray<T_Type, T_Capacity>*>(this)->operator[](index));
}

template<typename T_Type, size_t T_Capacity>
const T_Type &PushArray<T_Type, T_Capacity>::operator[](size_t index)const{
    SX_CORE_ASSERT(index < m_Size, "PushArray: Can't index more than PushArray::Size() elements");

    return begin()[index];
}

template<typename T_Type, size_t T_Capacity>
PushArray<T_Type, T_Capacity> &PushArray<T_Type, T_Capacity>::operator=(const PushArray &other){
    Clear();
    for(auto &e: other)
        Push(e);
    return *this;
}

template<typename T_Type, size_t T_Capacity>
PushArray<T_Type, T_Capacity> &PushArray<T_Type, T_Capacity>::operator=(PushArray &&other){
    Clear();
    for(auto &e: other)
        Emplace(Move(e)); // We assume that moved stuff does not required to be destroyed
    other.m_Size = 0;
    return *this;
}

template<typename T_Type, size_t T_Capacity>
PushArray<T_Type, T_Capacity>::operator Span<T_Type>(){
    return Span<T_Type>(m_Array.Data(), Size());
}

template<typename T_Type, size_t T_Capacity>
size_t PushArray<T_Type, T_Capacity>::Size()const{
    return m_Size;
}

template<typename T_Type, size_t T_Capacity>
size_t PushArray<T_Type, T_Capacity>::Capacity()const{
    return T_Capacity;
}

template<typename T_Type, size_t T_Capacity>
T_Type *PushArray<T_Type, T_Capacity>::Data(){
    return m_Array.Data();
}

template<typename T_Type, size_t T_Capacity>
const T_Type *PushArray<T_Type, T_Capacity>::Data()const{
    return m_Array.Data();
}

template<typename T_Type, size_t T_Capacity>
typename PushArray<T_Type, T_Capacity>::iterator PushArray<T_Type, T_Capacity>::begin(){
    return Data();
}

template<typename T_Type, size_t T_Capacity>
typename PushArray<T_Type, T_Capacity>::iterator PushArray<T_Type, T_Capacity>::end(){
    return begin()+Size();
}

template<typename T_Type, size_t T_Capacity>
typename PushArray<T_Type, T_Capacity>::const_iterator PushArray<T_Type, T_Capacity>::begin()const{
    return Data();
}

template<typename T_Type, size_t T_Capacity>
typename PushArray<T_Type, T_Capacity>::const_iterator PushArray<T_Type, T_Capacity>::end()const{
    return begin()+Size();
}

}//namespace StraitX::

#endif//STRAITX_PUSH_ARRAY_HPP