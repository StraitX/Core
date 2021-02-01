#ifndef STRAITX_TEMPLATE_HPP
#define STRAITX_TEMPLATE_HPP

namespace StraitX{

template <typename T_Type, T_Type T_Value>
struct IntegralConstant{
    static constexpr T_Type Value = T_Value;

    constexpr operator T_Type()const{
        return T_Value;
    }

    constexpr T_Type operator()()const{
        return T_Value;
    }
};

template <typename T>
struct RemoveConst{
    typedef T Type;
};

template <typename T>
struct RemoveConst<const T>{
    typedef T Type; 
};

template <typename T>
struct IsConst: IntegralConstant<bool, false>{};

template <typename T>
struct IsConst<const T>: IntegralConstant<bool, true>{};

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