#ifndef STRAITX_ALGORITHM_HPP
#define STRAITX_ALGORITHM_HPP

#include "core/move.hpp"

namespace StraitX{

template <typename T>
constexpr void Swap(T& f, T& s){
    T tmp = Move(f);
    f = Move(s);
    s = Move(tmp);
}

template<typename T>
constexpr const T &Min(const T &a, const T &b){
	if(a < b)
		return a;
	return b;
}

template<typename T>
constexpr const T &Max(const T &a, const T &b){
	if(a > b)
		return a;
	return b;
}


template<typename FwdIt, typename T>
constexpr FwdIt Find(FwdIt begin, FwdIt end, const T& value){
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



} // namespace StraitX::

#endif // STRAITX_ALGORITHM_HPP