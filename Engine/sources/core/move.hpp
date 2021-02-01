#ifndef STRAITX_MOVE_HPP
#define STRAITX_MOVE_HPP

#include "core/template.hpp"

namespace StraitX{

template <typename T>
constexpr typename RemoveReference<T>::Type&& Move(T&& t){
    return static_cast<typename RemoveReference<T>::Type&&>(t);
}

}// namespace StraitX::

#endif // STRAITX_MOVE_HPP