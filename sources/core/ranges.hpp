#ifndef STRAITX_RANGES_HPP
#define STRAITX_RANGES_HPP

#include "core/iterators/indexed.hpp"
#include "core/iterators/reverse.hpp"
#include "core/printer.hpp"
#include "core/type_traits.hpp"
#include "core/assert.hpp"

template<typename BaseIt>
struct Range{
    const BaseIt Begin;
    const BaseIt End;

    Range(BaseIt begin, BaseIt end):
        Begin(begin),
        End(end) 
    {}

    BaseIt begin()const{
        return Begin;
    }

    BaseIt end()const{
        return End;
    }
};

template <typename Range, typename = decltype(Declval<Range>().begin())>
auto Begin(Range &range) {
    return range.begin();
}

template <typename Range, typename = decltype(Declval<Range>().end())>
auto End(Range &range) {
    return range.end();
}

template <typename Type, size_t SizeValue>
Type *Begin(Type (&range)[SizeValue]) {
    return range;
}

template <typename Type, size_t SizeValue>
Type *End(Type (&range)[SizeValue]) {
    return range + SizeValue;
}
template <typename Type, size_t SizeValue>
Type *Begin(Type (range)[SizeValue]) {
    return range;
}

template <typename Type, size_t SizeValue>
Type *End(Type (range)[SizeValue]) {
    return range + SizeValue;
}

template <typename RangeLikeType>
auto ToRange(RangeLikeType &type)->Range<decltype(Begin(type))>{
    return {Begin(type), End(type)};
}

template<typename Type>
class IsRange{
private:
    using BaseType = typename RemoveConstVolatile<typename RemoveReference<Type>::Type>::Type;
    
    static constexpr bool Check(...){
        return false;
    }

    template<typename _BaseType = BaseType, typename = decltype(ToRange(Declval<_BaseType>()))>
    static constexpr bool Check(void *){
        return true;
    }
public:
    static_assert(!IsPointer<BaseType>::Value, "Underlying type can't be a pointer");

    static constexpr bool Value = Check(nullptr);
};


template<typename T>
struct Printer<Range<T>>{
	static void Print(const Range<T> &range, StringWriter &writer){
        writer.Write('[');
        auto begin = range.begin(), end = range.end();
        for(;;){
            using TypePrinter = Printer<typename RemoveConstVolatile<typename RemoveReference<decltype(*begin)>::Type>::Type>;
            TypePrinter::Print(*begin, writer);

            auto tmp = begin;
            if(!(++tmp != end))
                break;

            writer.Write(',');
            writer.Write(' ');

            ++begin;
        }
        writer.Write(']');
    }
};

template<typename RangeType> 
auto ForwardRange(RangeType &range) -> Range<decltype(Begin(range))>{
    return {Begin(range), End(range)};
}

template<typename RangeType>
auto ReverseRange(RangeType &range) -> Range<ReverseIterator<decltype(Begin(range))>>{
    //XXX: check if range has rbegin and rend methods
    auto begin = End(range);
    auto end = Begin(range);
    return {--begin, --end};
}

template<typename RangeType>
auto IndexedRange(RangeType &range) -> Range<IndexedIterator<decltype(Begin(range))>>{
    return {Begin(range), End(range)};
}

template<typename T>
class IntRangeIterator{
    T m_Value;
    T m_Increment;
public:
	IntRangeIterator() = default;

    IntRangeIterator(T v, T inc): 
        m_Value(v),
        m_Increment(inc)
    {}

    const IntRangeIterator& operator++() { 
        m_Value += m_Increment; 
        return *this;
    }

    bool operator!=(const IntRangeIterator & o) { 
        return o.m_Value != m_Value;
    }

    T operator*() const { 
        return m_Value;
    }
};

template<typename T>
auto IntRange(T begin, T end, T inc = T(1)) {
    auto abs = [](T num) {
        if(num < 0)
            return -num;
        return num;
    };
    SX_CORE_ASSERT(begin <= end, "begin should be less or equel end");
    SX_CORE_ASSERT(inc != 0, "inc should not be null");
    SX_CORE_ASSERT(abs(end - begin) % inc == 0, "inc should direct into end");
    return Range<IntRangeIterator<T>>(
        {begin, inc},
        {end, inc}
    );
}

template<typename T>
auto IntRange(T end) {
    return IntRange(T(0), end);
}


#endif//STRAITX_RANGES_HPP