#ifndef STRAITX_FUNCTION_HPP
#define STRAITX_FUNCTION_HPP

#include "core/move.hpp"

template<typename T>
class Function;

template<typename ReturnType, typename ...ArgsType>
class Function<ReturnType(ArgsType...)>{
private:
    using Signature = ReturnType (*)(ArgsType...);

    using ProxyFunction = ReturnType (*)(void *, ArgsType...);

    static ReturnType FunctionProxy(void *ptr, ArgsType...args){
        return reinterpret_cast<Signature>(ptr)(Forward<ArgsType>(args)...);
    }

    template<typename ObjectType, ReturnType(ObjectType::*Method)(ArgsType...)>
    static ReturnType TemplateMethodProxy(void *ptr, ArgsType...args){
        return (reinterpret_cast<ObjectType*>(ptr)->*Method)(Forward<ArgsType>(args)...);
    }

    template<typename ObjectType, ReturnType(ObjectType::*Method)(ArgsType...)const>
    static ReturnType TemplateMethodProxy(void *ptr, ArgsType...args){
        return (reinterpret_cast<const ObjectType*>(ptr)->*Method)(Forward<ArgsType>(args)...);
    }

    template<Signature Function>
    static ReturnType TemplateFunctionProxy(void *, ArgsType...args){
        return reinterpret_cast<Signature>(Function)(Forward<ArgsType>(args)...);
    }

private:
    void *m_Ptr = nullptr;
    ProxyFunction m_Proxy = nullptr;
public:
    Function() = default;

    Function(const Function &other) = default;

    Function(Function &&other){
        *this = Move(other);
    }

    ~Function() = default;

    Function &operator=(const Function &other) = default;

    Function &operator=(Function &&other){
        m_Ptr = other.m_Ptr;
        m_Proxy = other.m_Proxy;

        other.Unbind();
    }

    bool operator==(const Function &other){
        return m_Proxy == other.m_Proxy && m_Ptr == other.m_Ptr;
    }

    bool operator!=(const Function &other){
        return m_Proxy != other.m_Proxy || m_Ptr != other.m_Ptr;
    }

    ReturnType operator()(ArgsType...args){
        assert(m_Proxy);

        return m_Proxy(m_Ptr, Forward<ArgsType>(args)...);
    }

    void Bind(Signature function_pointer){
        m_Ptr = reinterpret_cast<void*>(function_pointer);
        m_Proxy = FunctionProxy; 
    }

    template<typename ObjectType, ReturnType(ObjectType::*Method)(ArgsType...)>
    void Bind(ObjectType *object){
        m_Ptr = object;
        m_Proxy = TemplateMethodProxy<ObjectType, Method>;
    }

    template<typename ObjectType, ReturnType(ObjectType::*Method)(ArgsType...)const>
    void Bind(ObjectType *object){
        m_Ptr = object;
        m_Proxy = TemplateMethodProxy<ObjectType, Method>;
    }

    template<Signature Function>
    void Bind(){
        m_Ptr = nullptr;
        m_Proxy = TemplateFunctionProxy<Function>;
    }

    void Unbind(){
        m_Ptr = nullptr;
        m_Proxy = nullptr;
    }

    bool IsBound()const{
        return m_Proxy != nullptr;
    }

    void TryCall(ArgsType...args){
        if(m_Proxy)
            (void)operator()(Forward<ArgsType>(args)...);
    }
};

#endif//STRAITX_FUNCTION_HPP