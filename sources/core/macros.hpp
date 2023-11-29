#pragma once

#include "core/type_traits.hpp"

#define GEN_NON_CONST_GETTER_REF(ConstMethodCallExpr) \
{ \
    using ConstReturnType = decltype(this->ConstMethodCallExpr); \
    using ReturnType = typename RemoveConst<typename RemoveReference<ConstReturnType>::Type>::Type; \
    using ThisClass = typename RemoveConst<typename RemovePointer<decltype(this)>::Type>::Type; \
 \
    return const_cast<ReturnType&>(const_cast<const ThisClass*>(this)->ConstMethodCallExpr); \
}

#define GEN_NON_CONST_GETTER_PTR(ConstMethodCallExpr) \
{ \
    using ConstReturnType = decltype(this->ConstMethodCallExpr); \
    using ReturnType = typename RemoveConst<typename RemoveReference<ConstReturnType>::Type>::Type; \
    using ThisClass = typename RemoveConst<typename RemovePointer<decltype(this)>::Type>::Type; \
 \
    return const_cast<ReturnType*>(const_cast<const ThisClass*>(this)->ConstMethodCallExpr); \
}
