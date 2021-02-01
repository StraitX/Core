#ifndef STRAITX_TEMPLATE_HPP
#define STRAITX_TEMPLATE_HPP

namespace StraitX{

template <typename T>
struct RemoveConst{
    typedef T Type;
};

template <typename T>
struct RemoveConst<const T>{
    typedef T Type; 
};

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

}//namespace StraitX::

#endif //STRAITX_TEMPLATE_HPP