#ifndef STRAITX_ITERATORS_REVERSE_HPP
#define STRAITX_ITERATORS_REVERSE_HPP

#include "core/type_traits.hpp"
#include "core/templates.hpp"

template<typename BaseIt>
class ReverseIterator{
private:
    BaseIt It;
public:
    ReverseIterator(BaseIt it):
        It(it)
    {}

    template<typename _BaseIt = BaseIt>
    ReverseIterator &operator++(){
        It--;
        return *this;
    }

    template<typename _BaseIt = BaseIt>
    ReverseIterator &operator++(int){
        --It;
        return *this;
    }

    template<typename _BaseIt = BaseIt>
    ReverseIterator &operator--(){
        It++;
        return *this;
    }

    template<typename _BaseIt = BaseIt>
    ReverseIterator &operator--(int){
        ++It;
        return *this;
    }

    auto operator*(){
        return *It;
    }

    auto operator*()const{
        return *It;
    }

    auto operator->(){
        return &*It;
    }

    auto operator->()const{
        return &*It;
    }

    template<typename _BaseIt = BaseIt>
    bool operator==(const ReverseIterator &other){
        return this->It == other.It;
    }

    template<typename _BaseIt = BaseIt>
    bool operator!=(const ReverseIterator &other){
        return this->It != other.It;
    }
};

#endif//STRAITX_ITERATORS_REVERSE_HPP