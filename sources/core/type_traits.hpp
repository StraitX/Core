#ifndef STRAITX_TYPE_TRAITS_HPP
#define STRAITX_TYPE_TRAITS_HPP

template <typename Type>
constexpr Type &&Declval()noexcept;

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

template<typename SrcType, typename DstType>
class IsCastable{
private:
    static constexpr bool Check(...){
        return false;
    }

    template<typename = decltype(static_cast<DstType>(Declval<SrcType>()))>
    static constexpr bool Check(void *){
        return true;
    }
public:
    static constexpr bool Value = Check(nullptr);
};

template<typename Type>
class IsRange{
private:
    using BaseType = typename RemoveReference<typename RemoveConstVolatile<Type>::Type>::Type;
    
    static constexpr bool Check(...){
        return false;
    }

    template<typename = decltype(Declval<Type>().begin()), typename = decltype(Declval<Type>().end())>
    static constexpr bool Check(void *){
        return true;
    }
public:
    static_assert(!IsPointer<BaseType>::Value, "Underlying type can't be a pointer");

    static constexpr bool Value = Check(nullptr);
};

template<typename Type>
class IsReverseRange{
private:
    using BaseType = typename RemoveReference<typename RemoveConstVolatile<Type>::Type>::Type;

    static constexpr bool Check(...){
        return false;
    }

    template<typename = decltype(Declval<Type>().rbegin()), typename = decltype(Declval<Type>().rend())>
    static constexpr bool Check(void *){
        return true;
    }
public:
    static_assert(!IsPointer<BaseType>::Value, "Underlying type can't be a pointer");

    static constexpr bool Value = Check(nullptr);
};

template<typename Type, typename ...ArgsType>
class IsConstructible {
private:
    template<typename = decltype(Type(Declval<ArgsType>()...))>
    static constexpr bool Check(void*) {
        return true;
    }
    static constexpr bool Check(...) {
        return false;
    }
public:
    static constexpr bool Value = Check(nullptr);
};

template<typename Type>
using IsMoveConstructible = IsConstructible<Type, typename RemoveReference<Type>::Type &&>;

template<typename Type>
using IsCopyConstructible = IsConstructible<Type, const typename RemoveReference<Type>::Type &>;

template<typename Type, typename ...ArgsType>
class IsNothrowConstructible {
private:
    template<bool IsNoexcept = noexcept(Type(Declval<ArgsType>()...))>
    static constexpr bool Check(void*) {
        return IsNoexcept;
    }
    static constexpr bool Check(...) {
        return false;
    }
public:
    static constexpr bool Value = Check(nullptr);
};

template<typename Type>
using IsNothrowMoveConstructible = IsNothrowConstructible<Type, typename RemoveReference<Type>::Type &&>;

template<typename Type>
using IsNothrowCopyConstructible = IsNothrowConstructible<Type, const typename RemoveReference<Type>::Type &>;

#endif //STRAITX_TYPE_TRAITS_HPP