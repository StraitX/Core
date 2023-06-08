#ifndef STRAITX_ALGORITHM_HPP
#define STRAITX_ALGORITHM_HPP

#include "core/move.hpp"
#include "core/span.hpp"

template <typename Type>
constexpr void Swap(Type& f, Type& s){
    Type tmp = Move(f);
    f = Move(s);
    s = Move(tmp);
}

template<typename Type>
constexpr const Type &Min(const Type &a, const Type &b){
	if(a < b)
		return a;
	return b;
}

template<typename Type>
constexpr const Type &Max(const Type &a, const Type &b){
	if(a > b)
		return a;
	return b;
}


template<typename FwdIt, typename Type>
constexpr FwdIt Find(FwdIt begin, FwdIt end, const Type& value){
	for(; begin != end; begin++){
		if(*begin == value)
			return begin;
	}
	return begin;
}

template<typename FwdIt, typename Predicate>
constexpr FwdIt FindIf(FwdIt begin, FwdIt end, Predicate predicate){
	for(; begin != end; begin++){
		if(predicate(*begin))
			return begin;
	}
	return begin;
}

template<typename ElementType>
constexpr ElementType Sum(ConstSpan<ElementType> elements) {
	ElementType sum{};

	for(const auto &element: elements)
		sum += element;

	return sum;
}

template<typename ElementType>
constexpr auto Avg(ConstSpan<ElementType> elements) {
	return Sum(elements) / elements.Size();
}

#endif // STRAITX_ALGORITHM_HPP