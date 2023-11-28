#ifndef STRAITX_FUNCTION_HPP
#define STRAITX_FUNCTION_HPP

#include "core/move.hpp"
#include "core/assert.hpp"

template<typename T>
class Function;

template<typename ReturnType, typename ...ArgsType>
class Function<ReturnType(ArgsType...)>{
private:
    struct Object{ };

    using Signature = ReturnType (*)(ArgsType...);

    using CallableUnion = union {
        ReturnType (*Function)(ArgsType...);
        ReturnType (Object::*Method)(ArgsType...);
        ReturnType (Object::*ConstMethod)(ArgsType...)const;
    };

    using ProxyFunction = ReturnType (*)(void *, const CallableUnion &, ArgsType...);

    static ReturnType FunctionProxy(void *, const CallableUnion &callable, ArgsType...args){
        return reinterpret_cast<Signature>(callable.Function)(Forward<ArgsType>(args)...);
    }

    template<typename ObjectType>
    static ReturnType MethodProxy(void *obj, const CallableUnion &callable, ArgsType...args){
        auto method = reinterpret_cast<ReturnType(ObjectType::*)(ArgsType...)>(callable.Method);
        return (reinterpret_cast<ObjectType*>(obj)->*method)(Forward<ArgsType>(args)...);
    }

    template<typename ObjectType>
    static ReturnType ConstMethodProxy(void *obj, const CallableUnion &callable, ArgsType...args){
        auto method = reinterpret_cast<ReturnType(ObjectType::*)(ArgsType...)const>(callable.ConstMethod);
        return (reinterpret_cast<ObjectType*>(obj)->*method)(Forward<ArgsType>(args)...);
    }

private:
    CallableUnion m_Callable;
    void *m_ObjectPtr = nullptr;
    ProxyFunction m_Proxy = nullptr;
public:
    Function() = default;

    Function(Signature fptr){
        Bind(fptr);
    }

    template<typename Functor>
    Function(Functor functor){
        Signature fptr = functor;
        Bind(fptr);
    }

    template<typename ObjectType>
    Function(ObjectType *obj, ReturnType (ObjectType::*method)(ArgsType...)){
        Bind(obj, method);
    }

    template<typename ObjectType>
    Function(ObjectType *obj, ReturnType (ObjectType::*method)(ArgsType...)const){
        Bind(obj, method);
    }

    Function(const Function &other) = default;

    Function(Function &&other)noexcept{
        *this = Move(other);
    }

    ~Function() = default;

    Function &operator=(Signature fptr){
        Bind(fptr);
    }

    template<typename Functor>
    Function &operator=(Functor functor){
        Signature fptr = functor;
        Bind(fptr);
        return *this;        
    }

    Function &operator=(const Function &other) = default;

    Function &operator=(Function &&other)noexcept{
        m_Callable = other.m_Callable;
        m_ObjectPtr = other.m_ObjectPtr;
        m_Proxy = other.m_Proxy;

        other.Unbind();

        return *this;
    }

    bool operator==(const Function &other){
        return m_Proxy == other.m_Proxy && m_Callable == other.m_Callable && m_ObjectPtr == other.m_ObjectPtr;
    }

    bool operator!=(const Function &other){
        return !(*this == other);
    }

    ReturnType Call(ArgsType...args)const{
        SX_CORE_ASSERT(m_Proxy, "Can't call empty function");

        return m_Proxy(m_ObjectPtr, m_Callable, Forward<ArgsType>(args)...);
    }

    ReturnType operator()(ArgsType...args)const{
        return Call(Forward<ArgsType>(args)...);
    }

    Function &Bind(Signature function_pointer){
        m_ObjectPtr = nullptr;
        m_Callable.Function = function_pointer;
        m_Proxy = FunctionProxy;

        return *this;
    }

    template<typename ObjectType>
    Function &Bind(ObjectType *object, ReturnType(ObjectType::*method)(ArgsType...)){
        m_Callable.Method = reinterpret_cast<ReturnType(Object::*)(ArgsType...)>(method);
        m_ObjectPtr = object;
        m_Proxy = MethodProxy<ObjectType>;

        return *this;
    }

    template<typename ObjectType>
    Function &Bind(ObjectType *object, ReturnType(ObjectType::*method)(ArgsType...)const){
        m_Callable.ConstMethod = reinterpret_cast<ReturnType(Object::*)(ArgsType...)const>(method);
        m_ObjectPtr = object;
        m_Proxy = ConstMethodProxy<ObjectType>;

        return *this;
    }

    void Unbind(){
        m_Callable.Method = nullptr;
        m_Callable.ConstMethod = nullptr;
        m_Callable.Function = nullptr;
        m_ObjectPtr = nullptr;
        m_Proxy = nullptr;
    }

    bool IsBound()const{
        return m_Proxy != nullptr;
    }

    operator bool()const{
        return IsBound();
    }
    //XXX: use optional for return value
    void TryCall(ArgsType...args)const{
        if(IsBound())
            (void)Call(Forward<ArgsType>(args)...);
    }
};

#endif//STRAITX_FUNCTION_HPP