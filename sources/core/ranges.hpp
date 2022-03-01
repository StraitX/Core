#ifndef STRAITX_RANGES_HPP
#define STRAITX_RANGES_HPP

#include "core/iterators/indexed.hpp"
#include "core/iterators/reverse.hpp"
#include "core/printer.hpp"
#include "core/type_traits.hpp"

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
    using BaseType = typename RemoveReference<typename RemoveConstVolatile<Type>::Type>::Type;
    
    static constexpr bool Check(...){
        return false;
    }

    template<typename = decltype(ToRange(Declval<BaseType>()))>
    static constexpr bool Check(void *){
        return true;
    }
public:
    static_assert(!IsPointer<BaseType>::Value, "Underlying type can't be a pointer");

    static constexpr bool Value = Check(nullptr);
};


template<typename T>
struct Printer<Range<T>>{
	static void Print(const Range<T> &range, void (*writer)(char, void*), void *writer_data){
        writer('[', writer_data);
        auto begin = range.begin(), end = range.end();
        for(;;){
            using TypePrinter = Printer<typename RemoveConstVolatile<typename RemoveReference<decltype(*begin)>::Type>::Type>;
            TypePrinter::Print(*begin, writer, writer_data);

            auto tmp = begin;
            if(++tmp == end)
                break;

            writer(',', writer_data);
            writer(' ', writer_data);

            ++begin;
        }
        writer(']', writer_data);
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

#endif//STRAITX_RANGES_HPP