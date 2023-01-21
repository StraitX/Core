#pragma once

#include "core/move.hpp"
#include "core/type_traits.hpp"
#include "core/ranges.hpp"

//XXX: Add all required Trait checks
template <typename PredicateType>
class RangeToValueConvertor {
private:
	PredicateType m_Predicate;
public:
	RangeToValueConvertor(PredicateType predicate):
		m_Predicate(predicate)
	{}
	
	template<typename RangeType, typename _ = EnableIfType<IsRange<RangeType>::Value, void>>
	auto Convert(RangeType &&range)const{
		return m_Predicate(Forward<RangeType>(range));
	}
};

template <typename RangeType, typename PredicateType>
auto operator|(RangeType&& range, const RangeToValueConvertor<PredicateType>& range_to_value) {
	return range_to_value.Convert(Forward<RangeType>(range));
}

template <typename PredicateType>
auto RangeToValue(PredicateType predicate) {
	return RangeToValueConvertor<PredicateType>(predicate);
}

template <typename PredicateType>
auto FindByPredicate(const PredicateType &predicate) {
	return RangeToValue([=](auto&& range) {
		auto it = range.begin();
		auto end = range.end();

		for (; it != end; ++it) {
			if (predicate(*it))
				return it;
		}
		return it;
	});
}

template <typename ValueType>
auto Find(const ValueType &value) {
	return FindByPredicate([=](const auto& target) {
		return value == target;
	});
}

template <typename PredicateType>
auto ContainsByPredicate(const PredicateType &predicate) {
	return RangeToValue([=](auto&& range) {
		for (const auto& element : range)
			if (predicate(element))
				return true;
		return false;
	});
}

template <typename ValueType>
auto Contains(const ValueType &value) {
	return ContainsByPredicate([=](const auto &target) {
		return value == target;
	});
}
