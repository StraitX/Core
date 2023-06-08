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

template <typename ArrayType, size_t N, typename PredicateType>
auto operator|(ArrayType (&range)[N], const RangeToValueConvertor<PredicateType>& range_to_value) {
	return range_to_value.Convert(ToRange(range));
}

template <typename PredicateType>
auto RangeToValue(PredicateType predicate) {
	return RangeToValueConvertor<PredicateType>(predicate);
}

template <typename PredicateType>
auto FindByPredicate(const PredicateType &predicate) {
	return RangeToValue([=](auto&& range) {
		auto it = Begin(range);
		auto end = End(range);

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

template <typename PredicateType>
auto IndexOfByPredicate(const PredicateType& predicate) {
	return RangeToValue([=](auto&& range)->size_t{
		auto it = Begin(range);
		auto end = End(range);

		for (size_t i = 0; it != end; ++it, ++i) {
			if (predicate(*it))
				return i;
		}
		return -1;
	});
}

template <typename ValueType>
auto IndexOf(const ValueType& value) {
	return IndexOfByPredicate([=](const auto &target)->bool{
		return value == target;
	});
}

template <typename PredicateType>
auto CountByPredicate(const PredicateType& predicate) {
	return RangeToValue([=](auto&& range)->size_t {
		auto it = Begin(range);
		auto end = End(range);

		size_t count = 0;
		for (; it != end; ++it) {
			if (predicate(*it))
				count += 1;
		}

		return count;
	});
}

auto Count() {
	return CountByPredicate([](const auto&) {
		return true;
	});
}

template<typename ValueType>
auto Count(const ValueType &value) {
	return CountByPredicate([=](const auto& target) {
		return value == target;
	});
}

template<typename ValueType>
auto CountExcept(const ValueType &value) {
	return CountByPredicate([=](const auto& target) {
		return value != target;
	});
}


