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

} // namespace StraitX::

#endif // STRAITX_ALGORITHM_HPP