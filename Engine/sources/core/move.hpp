#ifndef STRAITX_MOVE_HPP
#define STRAITX_MOVE_HPP

#include "core/template.hpp"

template <typename T>
constexpr typename RemoveReference<T>::Type&& Move(T&& t){
    return static_cast<typename RemoveReference<T>::Type&&>(t);
}

template <typename T>
constexpr T&& Forward(typename RemoveReference<T>::Type&& elem){
    return static_cast<T&&>(elem);
}

template <typename T>
constexpr T&& Forward(typename RemoveReference<T>::Type& elem){
    return static_cast<T&&>(elem);
}

#endif // STRAITX_MOVE_HPP