#pragma once

#include "core/type_traits.hpp"
#include "core/move.hpp"
#include "core/ranges.hpp"

template<typename ParentIteratorType, typename PassOnTrueFilterType>
class FilteringIterator {
private:
	ParentIteratorType m_ParentCurrent;
	ParentIteratorType m_ParentEnd;
	PassOnTrueFilterType m_Filter;
public:
	FilteringIterator(ParentIteratorType current, ParentIteratorType end, PassOnTrueFilterType filter) :
		m_ParentCurrent(current),
		m_ParentEnd(end),
		m_Filter(filter)
	{
		Ignore();
	}

	auto &&operator*()const {
		return *m_ParentCurrent;
	}

	FilteringIterator& operator++() {
		++m_ParentCurrent;
		Ignore();

		return *this;
	}

	bool operator!=(const FilteringIterator& other)const {
		return m_ParentCurrent != other.m_ParentCurrent;
	}
private:
	void Ignore() {
		for (; m_ParentCurrent != m_ParentEnd && !m_Filter(*m_ParentCurrent); ++m_ParentCurrent);
	}
};

template <typename PassOnTrueFilterType>
class FilteredRange{
private:
	PassOnTrueFilterType m_Filter;
public:
	FilteredRange(PassOnTrueFilterType filter):
		m_Filter(Move(filter))
	{}
	
	template<typename RangeType, typename = EnableIfType<IsRange<RangeType>::Value, void>>
	auto MakeTypedRange(RangeType &&range) {
		using ParentRangeIteratorType = decltype(Declval<RangeType>().begin());
		using IteratorType = FilteringIterator<ParentRangeIteratorType, PassOnTrueFilterType>;
		return Range<IteratorType>{
			IteratorType(range.begin(), range.end(), m_Filter),
			IteratorType(range.end(), range.end(), m_Filter),
		};
	}
};

template<typename PassOnTrueFilterType>
auto Filtered(PassOnTrueFilterType filter) {
	return FilteredRange<PassOnTrueFilterType>(filter);
}

template<typename LeftRangeType, typename PassOnTrueFilterType>
auto operator|(LeftRangeType&& left, FilteredRange<PassOnTrueFilterType>&& right) {
	return right.MakeTypedRange(left);
}

