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

#endif//STRAITX_CORE_TEMPLATES_HPP