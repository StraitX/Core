#ifndef STRAITX_LIST_HPP
#define STRAITX_LIST_HPP

#include "core/type_traits.hpp"
#include "core/types.hpp"
#include "core/move.hpp"
#include "core/noncopyable.hpp"
#include "core/assert.hpp"
#include "core/allocators/allocator.hpp"
#include "core/span.hpp"
#include <initializer_list>

//TODO: 
// [ ] Optimize for Types with move ctors
// [ ] Optimize for Types without copy or mouse ctors
// [ ] Optimize for Types without dtor
// [ ] Make it copyable and movable
// [ ] Get rid of NonCopyable include XD

template<typename Type, typename GeneralAllocator = DefaultGeneralAllocator>
class List: private GeneralAllocator, public NonCopyable{
public:
    static_assert(!IsConst<Type>::Value && !IsVolatile<Type>::Value, "Type can't be const or volatile");
private:
    void *m_Memory = nullptr;
    size_t m_Size = 0;
    size_t m_Capacity = 0;
public:
    List() = default;

    List(ConstSpan<Type> span){
        Reserve(span.Size());

        for(const Type &element: span)
            Add(element);
    }

    List(Span<Type> span):
        List(ConstSpan<Type>(span.Pointer(), span.Size())) 
    {}

    List(std::initializer_list<Type> list):
        List(ConstSpan<Type>(list.begin(), list.size())) 
    {}

    ~List(){
        Free();
    }

    void Add(const Type &element){
        Emplace(element);
    }

    void Add(Type &&element){
        Emplace(Move(element));
    }

    template<typename...ArgsType>
    void Emplace(ArgsType...args){
        if(m_Size == m_Capacity)
            Reserve(m_Size * 2 + (m_Size == 0));

        new(&Data()[m_Size++]) Type(Forward<ArgsType>(args)...);
    }

    void RemoveLast(){
        SX_CORE_ASSERT(m_Size, "Can't remove last element from empty List");

        Data()[--m_Size].~Type();
    }

    void Reserve(size_t capacity){
        if(capacity <= m_Capacity)return;

        Type *new_array = (Type*)GeneralAllocator::Alloc(capacity * sizeof(Type));

        for(size_t i = 0; i<Size(); i++){
            new(&new_array[i]) Type(Data()[i]);
            Data()[i].~Type();
        }

        GeneralAllocator::Free(m_Memory);
        m_Memory = new_array;
        m_Capacity = capacity;
    }

    void Clear(){
        while(Size())
            RemoveLast();
    }

    void Free(){
        Clear();
        GeneralAllocator::Free(m_Memory);
        m_Memory = nullptr;
        m_Capacity = 0;
    }

    Type &operator[](size_t index){
        return const_cast<Type&>(const_cast<const List<Type, GeneralAllocator>*>(this)->operator[](index));
    }

    const Type &operator[](size_t index)const{
        SX_CORE_ASSERT(index < m_Size, "Invalid Index");

        return Data()[index];
    }

    operator Span<Type>(){
        return {Data(), Size()};
    }

    operator ConstSpan<Type>()const{
        return {Data(), Size()};
    }

    Type *Data(){
        return reinterpret_cast<Type*>(m_Memory);
    }

    const Type *Data()const{
        return reinterpret_cast<const Type*>(m_Memory);
    }

    size_t Size()const{
        return m_Size;
    }

    size_t Capacity()const{
        return m_Capacity;
    }

    Type *begin(){
        return Data();
    }

    Type *end(){
        return Data() + Size();
    }

    const Type *begin()const{
        return Data();
    }

    const Type *end()const{
        return Data() + Size();
    }
};

#endif//STRAITX_LIST_HPP