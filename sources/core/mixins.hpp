#pragma once

#include "core/unicode.hpp"
#include "core/span.hpp"
#include "core/assert.hpp"
#include "core/macros.hpp"
#include "core/move.hpp"

template <typename ArrayType, typename Type>
struct ArrayMixin {
    using ElementType = Type;
    using IteratorType = ElementType *;
    using ConstIteratorType = const ElementType *;

    ElementType *Data(){
        return ((ArrayType*)this)->Data();
    }

    const ElementType *Data()const {
        return ((const ArrayType*)this)->Data();
    }
    
    size_t Size()const {
        return ((const ArrayType*)this)->Size();
    }

    IteratorType begin(){
        return Data();
    }

    IteratorType end(){
        return Data() + Size();
    }

    ConstIteratorType begin()const{
        return Data();
    }

    ConstIteratorType end()const{
        return Data() + Size();
    }
    
    Span<ElementType> AsSpan() {
        return {Data(), Size()};
    }

    ConstSpan<ElementType> AsSpan()const{
        return {Data(), Size()};
    }

    operator Span<ElementType>(){
        return AsSpan();
    }

    operator ConstSpan<ElementType>()const{
        return AsSpan();
    }

    const ElementType &At(size_t index)const{
        SX_CORE_ASSERT(IsValidIndex(index), "Invalid Index");
        return Data()[index];
    }

    ElementType &At(size_t index) GEN_NON_CONST_GETTER_REF(At(index))

    const ElementType &First()const{
        return this->At(0);
    }

    ElementType &First() GEN_NON_CONST_GETTER_REF(First())

    const ElementType &Last()const{
        return this->At(Size() - 1);
    }

    ElementType &Last() GEN_NON_CONST_GETTER_REF(Last())

    ConstIteratorType Find(const ElementType& element)const{
        size_t index = IndexOf(element);

        return IsValidIndex(index) ? &At(index) : end();
    }

    IteratorType Find(const ElementType &element) GEN_NON_CONST_GETTER_PTR(Find(element))

    bool IsValidIndex(size_t index)const {
        return index < Size();
    }

    size_t IndexOf(const ElementType& element)const {
        for (size_t i = 0; i < Size(); i++) {
            if(At(i) == element)
                return i;
        }
        return -1;
    }

    bool Has(const ElementType &element)const{
        return IsValidIndex(IndexOf(element));
    }

    size_t Count(const ElementType& element)const {
        size_t count = 0;
        for (const auto& e : AsSpan()) {
		    if(e == element)
                count++;
        }
        return count;
    }

    template<
        typename _Type = ElementType,
        typename DelimiterType = _Type,
        typename _ = decltype(
            _Type(),
            Declval<_Type>() += Declval<_Type>(),
            Declval<_Type>() += Declval<DelimiterType>()
        )
    >
    ElementType Join(DelimiterType delimiter) {
        ElementType result();

        for (size_t i = 0; i < Size(); i++) {
            result += At(i);

            if (i == Size() - 1) {
                break;
            }

            result += delimiter;
        }

        return result;
    }
};

template <typename ListType, typename Type>
struct ListMixin: public ArrayMixin<ListType, Type>{
    using ElementType = typename ArrayMixin<ListType, Type>::ElementType;
    using IteratorType = typename ArrayMixin<ListType, Type>::IteratorType;
    using ConstIteratorType = typename ArrayMixin<ListType, Type>::ConstIteratorType;

    template<typename...ArgsType>
    void Emplace(ArgsType&&...args){
        ((ListType*)this)->Emplace(Forward<ArgsType>(args)...);
    }

    void Reserve(size_t capacity) {
        ((ListType*)this)->Reserve(capacity);
    }

    void RemoveLast() {
        ((ListType*)this)->RemoveLast();
    }

    void Add(const ElementType &element){
        Emplace(element);
    }

    void Add(ElementType &&element){
        Emplace(Move(element));
    }

    //TODO: better checking later
    template<typename RangeType>
    void Append(const RangeType& range) {
        for(const auto &e: range)
            Add(e);
    }

    void AddUnique(const ElementType &element){
        if(this->Has(element))
            return;

        Add(element);
    }

    void AddUnique(ElementType &&element){
        if(this->Has(element))
            return;

        Add(Move(element));
    }

    void RemoveLasts(size_t count){
        SX_CORE_ASSERT(count <= this->Size(), "Can't remove more elements than in list");

        if(count > this->Size())
            count = this->Size();

        for(size_t i = 0; i<count; i++)
            RemoveLast();
    }
    
    void UnorderedRemove(size_t index){
        SX_CORE_ASSERT(IsValidIndex(index), "Index is out of range");

        this->At(index) = Move(this->Last());
        RemoveLast();
    }

    bool UnorderedRemove(const ElementType &element){
        size_t index = this->IndexOf(element);
        if (this->IsValidIndex(index))
            return (UnorderedRemove(index), true);
        else
            return false;
    }

    void UnorderedRemove(ConstIteratorType iterator){
        SX_CORE_ASSERT(iterator >= this->begin() && iterator < this->end(), "iterator is out of range");
        UnorderedRemove(iterator - this->begin());
    }
};

template <typename StringType>
struct StringMixin {
	const char* Data()const {
		return ((StringType*)this)->Data();
	}

	size_t CodeunitsCount()const {
		return ((StringType*)this)->CodeunitsCount();
	}

    size_t Size()const {
        return CodeunitsCount();
    }

    UnicodeString Unicode()const {
        return UnicodeString(Data(), Size());
    }
    
    ConstSpan<char> AsSpan()const {
        return {Data(), Size()};
    }

	bool IsEmpty()const {
        return !Size();
    }

    size_t CodepointsCount()const {
        size_t counter = 0;
        for (u32 ch : Unicode())
            counter++;
        return counter;
    }

    size_t Count(char ch)const {
        size_t count = 0;
        for (char character: AsSpan()) {
            if(character == ch)
                count++; 
        }
        return count;
    }

    size_t Count(u32 ch)const {
        size_t count = 0;
        for (u32 character : Unicode()) {
            if(character == ch)
                count++; 
        }
        return count;
    }

    StringType Sub(size_t start, size_t count)const{
        SX_CORE_ASSERT(start + count <= Size(), "Out of range SubString operation");
        return {&Data()[start], count};
    }

    operator ConstSpan<char>()const {
        return Span();
    }

    operator UnicodeString()const {
		return Unicode();
	}

	static constexpr size_t StaticCodeunitsCount(const char* string) {
		if(!string)return 0;

		size_t counter = 0;
		while (*string++) 
			counter++;
		return counter;
	}
};