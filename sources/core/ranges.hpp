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
auto ForwardRange(RangeType &range) -> Range<decltype(range.begin())>{
    return {range.begin(), range.end()};
}

template<typename RangeType>
auto ReverseRange(RangeType &range) -> Range<ReverseIterator<decltype(range.begin())>>{
    //XXX: check if range has rbegin and rend methods
    auto begin = range.end();
    auto end = range.begin();
    return {--begin, --end};
}

template<typename RangeType>
auto IndexedRange(RangeType &range) -> Range<IndexedIterator<decltype(range.begin())>>{
    return {range.begin(), range.end()};
}

#endif//STRAITX_RANGES_HPP