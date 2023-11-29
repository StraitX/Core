#ifndef STRAITX_FIXED_LIST_HPP
#define STRAITX_FIXED_LIST_HPP

#include <new>
#include <initializer_list>
#include "core/types.hpp"
#include "core/assert.hpp"
#include "core/move.hpp"
#include "core/span.hpp"
#include "core/mixins.hpp"

template<typename Type, size_t CapacityValue>
class FixedList: public ListMixin<FixedList<Type, CapacityValue>, Type>{
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

        Type& operator[](size_t index) {
            return Data()[index];
        }

        const Type& operator[](size_t index)const{
            return Data()[index];
        }
	}m_Elements;
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

    void Clear(){
        while(Size())
            RemoveLast();
    }

    template<typename ...ArgsType>
    Type &Emplace(ArgsType&&...args){
        SX_CORE_ASSERT(m_Size < CapacityValue, "FixedList: Can't add an element, array is full");

        return *new(&m_Elements[m_Size++])Type(Forward<ArgsType>(args)...);
    }

    void RemoveLast() {
        m_Elements[--m_Size].~Type();
    }

    Type &operator[](size_t index){
        return this->At(index);
    }

    const Type &operator[](size_t index)const{
        return this->At(index);
    }

    FixedList &operator=(const FixedList &other){
        Clear();
        for(auto &e: other)
            this->Add(e);
        return *this;
    }

    FixedList &operator=(FixedList &&other){
        Clear();
        for(auto &e: other)
            Emplace(Move(e)); // We assume that moved stuff does not required to be destroyed
        other.Clear();
        return *this;
    }

    size_t Size()const{
        return m_Size;
    }

    size_t Capacity()const{
        return CapacityValue;
    }

    Type *Data(){
        return m_Elements.Data();
    }

    const Type *Data()const{
        return m_Elements.Data();
    }
};

#endif//STRAITX_FIXED_LIST_HPP