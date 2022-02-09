#ifndef STRAITX_DELEGATE_HPP
#define STRAITX_DELEGATE_HPP

#include "core/function.hpp"
#include "core/allocators/allocator.hpp"
#include "core/list.hpp"

template<typename...ArgsType>
class Delegate: public List<Function<void(ArgsType...)>>{
public:
    using Subscriber = Function<void(ArgsType...)>;
public:
    Delegate() = default;

    ~Delegate() = default;

    template<typename ObjectType>
    Delegate &Bind(ObjectType *object, void(ObjectType::*method)(ArgsType...)){
        return Bind(Subscriber(object, method));
    }

    template<typename ObjectType>
    Delegate &Bind(ObjectType *object, void(ObjectType::*method)(ArgsType...)const){
        return Bind(Subscriber(object, method));
    }

    Delegate &Bind(Function<void(ArgsType...)> subscriber){
        List<Function<void(ArgsType...)>>::Add(subscriber);
        return *this;
    }

    void Call(ArgsType...args){
        for(auto &sub: *this)
            sub(Forward<ArgsType>(args)...);
    }

    void operator()(ArgsType...args){
        Call(Forward<ArgsType>(args)...);
    }
};

#endif//STRAITX_DELEGATE_HPP