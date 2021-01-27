#ifndef STRAITX_MOVE_HPP
#define STRAITX_MOVE_HPP

namespace StraitX{

template <typename T>
struct RemoveReference{
    typedef T Type;
};

template <typename T>
struct RemoveReference<T&>{
    typedef T Type;
};

template <typename T>
struct RemoveReference<T&&>{
    typedef T Type;
};

template <typename T>
constexpr typename RemoveReference<T>::Type&& Move(T&& t){
    return static_cast<typename RemoveReference<T>::Type&&>(t);
}


}// namespace StraitX::

#endif // STRAITX_MOVE_HPP