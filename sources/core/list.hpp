#ifndef STRAITX_LIST_HPP
#define STRAITX_LIST_HPP

#include "core/type_traits.hpp"
#include "core/templates.hpp"
#include "core/types.hpp"
#include "core/move.hpp"
#include "core/assert.hpp"
#include "core/allocators/allocator.hpp"
#include "core/span.hpp"
#include "core/algorithm.hpp"
#include "core/mixins.hpp"
#include <initializer_list>

//TODO: 
// [ ] Optimize for Types without copy or mouse ctors

template<typename Type, typename GeneralAllocator = DefaultGeneralAllocator>
class List: public ListMixin<List<Type>, Type>, private GeneralAllocator{
public:
    static_assert(!IsConst<Type>::Value && !IsVolatile<Type>::Value, "Type can't be const or volatile");

    using Iterator = Type *;
    using ConstIterator = const Type *;
    using ElementType = Type;

    using value_type = Type;
    using allocator_type = GeneralAllocator;
    using reference = Type&;
    using const_reference = const Type&;
    using pointer = Type *;
    using const_pointer = const Type *;
    using iterator = Iterator;
    using const_iterator = ConstIterator;
    using size_type = size_t;
private:
    Type *m_Elements = nullptr;
    size_t m_Size = 0;
    size_t m_Capacity = 0;
public:
    List() = default;

    List(Span<Type> span):
        List(ConstSpan<Type>(span.Pointer(), span.Size())) 
    {}

    template <typename RangeType>
    List(const RangeType &range){
        for(const auto &e: range) 
            Emplace(e);
    }

    template <typename EntryType>
    List(std::initializer_list<EntryType> list){
        Reserve(list.size());
        
        for(const EntryType &element: list)
            Emplace(element);
    }

    List(std::initializer_list<Type> list):
        List(ConstSpan<Type>(list.begin(), list.size()))
    {}

    List(List<Type> &&other) {
        *this = Move(other);
    }

    List(const List<Type> &other):
        List(ConstSpan<Type>(other.Data(), other.Size()))
    {}

    ~List(){
        Free();
    }

    List &operator=(List<Type> &&other) {
        Free();
        Swap(other);
        return *this;
    }

    List &operator=(const List<Type> &other) {
        Free();
        for (const auto& e : other) {
            Add(e);
        }
        return *this;
    }

    template<typename...ArgsType>
    void Emplace(ArgsType&&...args){
        if(m_Size == m_Capacity)
            Reserve(m_Size * 2 + (m_Size == 0));

        new(&Data()[m_Size++]) Type(Forward<ArgsType>(args)...);
    }

    void RemoveLast(){
        SX_CORE_ASSERT(m_Size, "Can't remove last element from empty List");

        m_Elements[--m_Size].~Type();
    }

    void Reserve(size_t capacity){
        if(capacity <= m_Capacity)return;

        Type *new_elements = (Type*)GeneralAllocator::Alloc(capacity * sizeof(Type));

        for(size_t i = 0; i<Size(); i++){
            new(&new_elements[i]) Type(Move(m_Elements[i]));
            m_Elements[i].~Type();
        }

        GeneralAllocator::Free(m_Elements);
        m_Elements = new_elements;
        m_Capacity = capacity;
    }

    void Swap(List<Type> &other) {
        ::Swap(m_Elements, other.m_Elements);
        ::Swap(m_Size, other.m_Size);
        ::Swap(m_Capacity, other.m_Capacity);
    }

    void Clear(){
        if(IsTriviallyDestructable<Type>::Value) {
            m_Size = 0;
        }else{
            while(Size())
                RemoveLast();
        }
    }

    void Free(){
        //Size should be zero
        Clear();
        GeneralAllocator::Free(m_Elements);
        m_Elements = nullptr;
        m_Capacity = 0;
    }

    Type &operator[](size_t index){
        return this->At(index);
    }

    const Type &operator[](size_t index)const{
        return this->At(index);
    }

    Type *Data(){
        return m_Elements;
    }

    const Type *Data()const{
        return m_Elements;
    }

    size_t Size()const{
        return m_Size;
    }

    size_t Capacity()const{
        return m_Capacity;
    }

private:
    template<typename _Type = Type, typename = typename EnableIf<IsMoveConstructible<_Type>::Value>::Type>
    static void MoveElseCopyCtorImpl(Type *dst, Type *src, void *) {
        new(dst) Type(Move(*src));
    }

    template<typename _Type = Type, typename = typename EnableIf<!IsMoveConstructible<_Type>::Value>::Type>
    static void MoveElseCopyCtorImpl(Type *dst, Type *src, ...) {
        new(dst) Type(*src);
    }
    static void MoveElseCopyCtor(Type *dst, Type *src) {
        MoveElseCopyCtorImpl(dst, src, nullptr);
    }
};

template<typename T, typename LeftAllocator, typename RightAllocator>
bool operator==(const List<T, LeftAllocator>& left, const List<T, RightAllocator>& right) {
    if(left.Size() != right.Size())
        return false;

    for (size_t i = 0; i < left.Size(); i++) {
        if(left[i] != right[i])
            return false;
    }

    return true;
}

template<typename T, typename LeftAllocator, typename RightAllocator>
bool operator!=(const List<T, LeftAllocator>& left, const List<T, RightAllocator>& right) {
    return !(left == right);
}

#endif//STRAITX_LIST_HPP