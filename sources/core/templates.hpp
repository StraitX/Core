#ifndef STRAITX_CORE_TEMPLATES_HPP
#define STRAITX_CORE_TEMPLATES_HPP

template<bool ConditionValue, typename ConditionType = void>
struct EnableIf{};

template<typename ConditionType>
struct EnableIf<true, ConditionType>{
    using Type = ConditionType;
};

template<bool ConditionValue, typename ConditionType = void>
using EnableIfType = typename EnableIf<ConditionValue, ConditionType>::Type;

template<bool ConditionValue, typename TrueType, typename FalseType>
struct TypeCondition {
    using Type = FalseType;
};

template<typename TrueType, typename FalseType>
struct TypeCondition <true, TrueType, FalseType>{
    using Type = TrueType;
};

#endif//STRAITX_CORE_TEMPLATES_HPP