#ifndef STRAITX_TYPE_TRAITS_HPP
#define STRAITX_TYPE_TRAITS_HPP

template <typename Type, Type TypeValue>
struct IntegralConstant{
    static constexpr Type Value = TypeValue;

    constexpr operator Type()const{
        return TypeValue;
    }

    constexpr Type operator()()const{
        return TypeValue;
    }
};

template <typename T>
struct RemoveConst{
    using Type = T;
};

template <typename T>
struct RemoveConst<const T>{
    using Type = T;
};

template<typename T>
struct RemoveVolatile{
    using Type = T;
};

template<typename T>
struct RemoveVolatile<volatile T>{
    using Type = T;
};

template<typename T>
struct RemoveConstVolatile{
    using Type = typename RemoveConst<typename RemoveVolatile<T>::Type>::Type;
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

template <typename T>
struct IsConst: IntegralConstant<bool, false>{};

template <typename T>
struct IsConst<const T>: IntegralConstant<bool, true>{};

template <typename T>
struct IsVolatile: IntegralConstant<bool, false>{};

template <typename T>
struct IsVolatile<volatile T>: IntegralConstant<bool, true>{};

template <typename T>
struct IsPointer: IntegralConstant<bool, false>{};

template <typename T>
struct IsPointer<T *>: IntegralConstant<bool, true>{};

template <typename Base, typename Derived>
class IsBaseOf{
private:
	static constexpr Derived *Pointer(){
		return nullptr;
	}

	static constexpr bool Check(Base *){
		return true;
	}

	static constexpr bool Check(void *){
		return false;
	}
public:
	static constexpr bool Value = Check(Pointer());
};

template<typename T>
class IsRange{
private:
    using BaseType = typename RemoveReference<typename RemoveConstVolatile<T>::Type>::Type;

    template<typename Type>
    static constexpr bool Check(...){
        return false;
    }

    template<typename Type>
    static constexpr bool Check(decltype(&Type::begin), decltype(&Type::end)){
        return true;
    }
public:
    static_assert(!IsPointer<BaseType>::Value, "Underlying type can't be a pointer");

    static constexpr bool Value = Check<BaseType>(nullptr, nullptr);
};


#endif //STRAITX_TYPE_TRAITS_HPP