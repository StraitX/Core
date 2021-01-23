#ifndef STRAITX_PAIR_HPP
#define STRAITX_PAIR_HPP

#include "core/move.hpp"

namespace StraitX{

template<typename F, typename S>
struct Pair{
    F First  = {};
    S Second = {};

    constexpr Pair();

    constexpr Pair(const F &f, const S &s);

    constexpr Pair(F &&f, S &&s);

    constexpr Pair(const Pair<F, S> &other);

    constexpr Pair(Pair<F, S> &&other);

    constexpr Pair &operator=(const Pair<F, S> &other);

    constexpr Pair &operator=(Pair<F, S> &&other);
};

template<typename F, typename S>
constexpr Pair<F,S>::Pair():
    First(),
    Second()
{}

template<typename F, typename S>
constexpr Pair<F,S>::Pair(const F &f, const S &s):
    First(f),
    Second(s)
{}

template<typename F, typename S>
constexpr Pair<F,S>::Pair(F &&f, S &&s):
    First(Move(f)),
    Second(Move(s))
{}

template<typename F, typename S>
constexpr Pair<F,S>::Pair(const Pair<F, S> &other):
    First(other.First),
    Second(other.Second)
{}

template<typename F, typename S>
constexpr Pair<F,S>::Pair(Pair<F, S> &&other):
    First(Move(other.First)),
    Second(Move(other.Second))
{}

template<typename F, typename S>
constexpr Pair<F, S> &Pair<F,S>::operator=(Pair<F, S> &&other){
    First = Move(other.First);
    Second = Move(other.Second);
    return *this;
}

template<typename F, typename S>
constexpr Pair<F, S> &Pair<F,S>::operator=(const Pair<F, S> &other){
    First = other.First;
    Second = other.Second;
    return *this;
}


}//namespace StraitX::

#endif // STRAITX_PAIR_HPP