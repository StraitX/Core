#ifndef STRAITX_DELEGATE_HPP
#define STRAITX_DELEGATE_HPP

#include "core/function.hpp"
#include "core/allocators/allocator.hpp"
#include "core/array.hpp"

template<typename...ArgsType>
class Delegate: public Array<Function<void(ArgsType...)>>{
public:
    using Subscriber = Function<void(ArgsType...)>;
public:
    Delegate() = default;

    ~Delegate() = default;

    void Bind(void (*function_pointer)(ArgsType...)){
        Bind(Subscriber().Bind(function_pointer)); 
    }

    template<typename ObjectType, void(ObjectType::*Method)(ArgsType...)>
    void Bind(ObjectType *object){
        Bind(Subscriber().template Bind<ObjectType, Method>(object));
    }

    template<typename ObjectType, void(ObjectType::*Method)(ArgsType...)const>
    void Bind(ObjectType *object){
        Bind(Subscriber().template Bind<ObjectType, Method>(object));
    }

    template<void(*FunctionValue)(ArgsType...)>
    void Bind(){
        Bind(Subscriber().template Bind<FunctionValue>());
    }

    void Bind(Function<void(ArgsType...)> subscriber){
        Add(subscriber);
    }

    void operator()(ArgsType...args){
        for(auto &sub: *this)
            sub(Forward<ArgsType>(args)...);
    }
};

#endif//STRAITX_DELEGATE_HPP