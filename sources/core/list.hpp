#ifndef STRAITX_LIST_HPP
#define STRAITX_LIST_HPP

#include "core/type_traits.hpp"
#include "core/templates.hpp"
#include "core/types.hpp"
#include "core/move.hpp"
#include "core/noncopyable.hpp"
#include "core/assert.hpp"
#include "core/allocators/allocator.hpp"
#include "core/span.hpp"
#include "core/algorithm.hpp"
#include <initializer_list>

//TODO: 
// [ ] Optimize for Types without copy or mouse ctors
// [ ] Get rid of NonCopyable include XD

template<typename Type, typename GeneralAllocator = DefaultGeneralAllocator>
class List: private GeneralAllocator, public NonCopyable{
public:
    static_assert(!IsConst<Type>::Value && !IsVolatile<Type>::Value, "Type can't be const or volatile");

    using Iterator = Type *;
    using ConstIterator = const Type *;
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

    template <typename EntryType>
    List(std::initializer_list<EntryType> list){
        Reserve(list.size());
        
        for(const EntryType &element: list)
            Emplace(element);
    }

    List(List<Type> &&other) {
        *this = Move(other);
    }

    ~List(){
        Free();
    }

    List &operator=(List<Type> &&other) {
        Free();
        Swap(other);
        return *this;
    }

    void Add(const Type &element){
        Emplace(element);
    }

    void Add(Type &&element){
        Emplace(Move(element));
    }

    template<typename...ArgsType>
    void Emplace(ArgsType&&...args){
        if(m_Size == m_Capacity)
            Reserve(m_Size * 2 + (m_Size == 0));

        new(&Data()[m_Size++]) Type{Forward<ArgsType>(args)...};
    }
    
    template<typename OtherGeneralAllocator>
    void Append(const List<Type, OtherGeneralAllocator>& other) {
        for(const auto &element: other)
            Add(element);
    }

    void RemoveLast(){
        SX_CORE_ASSERT(m_Size, "Can't remove last element from empty List");

        Data()[--m_Size].~Type();
    }

    void UnorderedRemove(size_t index){
        SX_CORE_ASSERT(index < m_Size, "Index is out of range");

        Data()[index] = Move(Last());
        RemoveLast();
    }

    void UnorderedRemove(const Type &type){
        for(size_t i = 0; i<Size(); i++){
            if(Data()[i] == type){
                UnorderedRemove(i);
                break;
            }
        }
    }

    void UnorderedRemove(ConstIterator *iterator){
        SX_CORE_ASSERT(iterator >= begin() && iterator < end(), "iterator is out of range");
        UnorderedRemove(iterator - begin());
    }

    void Reserve(size_t capacity){
        if(capacity <= m_Capacity)return;

        Type *new_array = (Type*)GeneralAllocator::Alloc(capacity * sizeof(Type));

        for(size_t i = 0; i<Size(); i++){
            MoveElseCopyCtor(&new_array[i], &Data()[i]);
            Data()[i].~Type();
        }

        GeneralAllocator::Free(m_Memory);
        m_Memory = new_array;
        m_Capacity = capacity;
    }

    void Swap(List<Type> &other) {
        ::Swap(m_Memory, other.m_Memory);
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

    Type &First(){
        return operator[](0);
    }

    const Type &First()const{
        return operator[](0);
    }

    Type &Last(){
        return operator[](Size() - 1);
    }

    const Type &Last()const{
        return operator[](Size() - 1);
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

#endif//STRAITX_LIST_HPP