#ifndef STRAITX_PUSH_ARRAY_HPP
#define STRAITX_PUSH_ARRAY_HPP

#include <new>
#include <initializer_list>
#include "core/types.hpp"
#include "core/assert.hpp"
#include "core/move.hpp"
#include "core/span.hpp"

template<typename Type, size_t CapacityValue>
class PushArray{
public:
    typedef Type * Iterator;
    typedef const Type * ConstIterator;
private:
    // we don't want c++ to construct objects for us
    size_t m_Size = 0;
	class alignas(Type){
	private:
    	u8 m_Memory[CapacityValue * sizeof(Type)];
	public:
		Type *Data(){
			return reinterpret_cast<Type*>(m_Memory);
		}

		const Type *Data()const{
			return reinterpret_cast<const Type*>(m_Memory);
		}
	}m_Array;
public:
    static_assert(!IsConst<Type>() && !IsVolatile<Type>(), "Type can't be cv-qualified");

    PushArray() = default;

    PushArray(const PushArray &other){
        *this = other;
    }

    PushArray(PushArray &&other){
        *this = Move(other);
    }

    PushArray(std::initializer_list<Type> initializer_list):
        m_Size(initializer_list.size())
    {
        SX_CORE_ASSERT(initializer_list.size() <= CapacityValue, "Initializer list is bigger than PushArray capacity");

        size_t i = 0;
        for(auto &e: initializer_list){
            operator[](i) = e;
            ++i;
        }
    }

    ~PushArray(){
        Clear();
    }

    void Add(const Type &element){
        Emplace(element);
    }

    void Add(Type &&element){
        Emplace(Move(element));
    }

    void RemoveLast(){
        begin()[--m_Size].~Type();
    }

    void Clear(){
        while(Size())
            RemoveLast();
    }

    Iterator Find(const Type &element){
        return const_cast<Iterator>(const_cast<const PushArray<Type, CapacityValue>*>(this)->Find(element));
    }

    ConstIterator Find(const Type &element)const{
        auto it = begin();
        for(;it != end(); ++it){
            if(*it == element)
                return it;
        }
        return it;
    }


    template<typename ...ArgsType>
    Type &Emplace(ArgsType&&...args){
        SX_CORE_ASSERT(m_Size < CapacityValue, "PushArray: Can't add an element, array is full");

        return *new(&begin()[m_Size++])Type(Forward<ArgsType>(args)...);
    }

    Type &operator[](size_t index){
        return const_cast<Type &>(const_cast<const PushArray<Type, CapacityValue>*>(this)->operator[](index));
    }

    const Type &operator[](size_t index)const{
        SX_CORE_ASSERT(index < m_Size, "PushArray: Can't index more than PushArray::Size() elements");

        return begin()[index];
    }

    PushArray &operator=(const PushArray &other){
        Clear();
        for(auto &e: other)
            Add(e);
        return *this;
    }

    PushArray &operator=(PushArray &&other){
        Clear();
        for(auto &e: other)
            Emplace(Move(e)); // We assume that moved stuff does not required to be destroyed
        other.m_Size = 0;
        return *this;
    }

    operator Span<Type>(){
        return {m_Array.Data(), Size()};
    }

    operator ConstSpan<Type>()const{
        return {m_Array.Data(), Size()};
    }

    size_t Size()const{
        return m_Size;
    }

    size_t Capacity()const{
        return CapacityValue;
    }

    Type *Data(){
        return m_Array.Data();
    }

    const Type *Data()const{
        return m_Array.Data();
    }

    Iterator begin(){
        return Data();
    }

    Iterator end(){
        return Data() + Size();
    }

    ConstIterator begin()const{
        return Data();
    }

    ConstIterator end()const{
        return Data() + Size();
    }
};

#endif//STRAITX_PUSH_ARRAY_HPP