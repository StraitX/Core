#pragma once

#include "core/type_traits.hpp"
#include "core/move.hpp"
#include "core/ranges.hpp"

template<typename ParentIteratorType, typename TransformFuncType>
class TransformingIterator {
private:
	ParentIteratorType m_ParentCurrent;
	TransformFuncType m_TransformFunc;
public:
	TransformingIterator(ParentIteratorType current, TransformFuncType transform_func) :
		m_ParentCurrent(current),
		m_TransformFunc(transform_func)
	{}

	auto operator*()const {
		return m_TransformFunc(*m_ParentCurrent);
	}

	TransformingIterator& operator++() {
		++m_ParentCurrent;
		return *this;
	}

	bool operator!=(const TransformingIterator& other)const {
		return m_ParentCurrent != other.m_ParentCurrent;
	}
};

template <typename TransformFuncType>
class TransformedRange{
private:
	TransformFuncType m_TransformFunc;
public:
	TransformedRange(TransformFuncType func):
		m_TransformFunc(Move(func))
	{}
	
	template<typename RangeType, typename = EnableIfType<IsRange<RangeType>::Value, void>>
	auto MakeTypedRange(RangeType &&range) {
		using ParentRangeIteratorType = decltype(Declval<RangeType>().begin());
		using IteratorType = TransformingIterator<ParentRangeIteratorType, TransformFuncType>;
		return Range<IteratorType>{
			IteratorType(range.begin(), m_TransformFunc),
			IteratorType(range.end(), m_TransformFunc),
		};
	}
};

template<typename TransformFuncType>
auto Transformed(TransformFuncType filter) {
	return TransformedRange<TransformFuncType>(filter);
}

template<typename LeftRangeType, typename TransformFuncType>
auto operator|(LeftRangeType&& left, TransformedRange<TransformFuncType>&& right) {
	return right.MakeTypedRange(left);
}
