#ifndef STRAITX_FIXED_LIST_HPP
#define STRAITX_FIXED_LIST_HPP

#include <new>
#include <initializer_list>
#include "core/types.hpp"
#include "core/assert.hpp"
#include "core/move.hpp"
#include "core/span.hpp"

template<typename Type, size_t CapacityValue>
class FixedList{
    static_assert(!IsConst<Type>() && !IsVolatile<Type>(), "Type can't be cv-qualified");
public:
    using Iterator = Type *;
    using ConstIterator = const Type *;
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

    FixedList() = default;

    FixedList(const FixedList &other){
        *this = other;
    }

    FixedList(FixedList &&other){
        *this = Move(other);
    }

    FixedList(std::initializer_list<Type> initializer_list):
        m_Size(initializer_list.size())
    {
        SX_CORE_ASSERT(initializer_list.size() <= CapacityValue, "Initializer list is bigger than FixedList capacity");

        size_t i = 0;
        for(auto &e: initializer_list){
            operator[](i) = e;
            ++i;
        }
    }

    ~FixedList(){
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
        return const_cast<Iterator>(const_cast<const FixedList<Type, CapacityValue>*>(this)->Find(element));
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
        SX_CORE_ASSERT(m_Size < CapacityValue, "FixedList: Can't add an element, array is full");

        return *new(&begin()[m_Size++])Type(Forward<ArgsType>(args)...);
    }

    Type &operator[](size_t index){
        return const_cast<Type &>(const_cast<const FixedList<Type, CapacityValue>*>(this)->operator[](index));
    }

    const Type &operator[](size_t index)const{
        SX_CORE_ASSERT(index < m_Size, "FixedList: Can't index more than FixedList::Size() elements");

        return begin()[index];
    }

    FixedList &operator=(const FixedList &other){
        Clear();
        for(auto &e: other)
            Add(e);
        return *this;
    }

    FixedList &operator=(FixedList &&other){
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

#endif//STRAITX_FIXED_LIST_HPP