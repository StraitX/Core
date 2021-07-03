#ifndef STRAITX_VALIDABLE_HPP
#define STRAITX_VALIDABLE_HPP

struct Validable{
    virtual bool IsValid()const = 0;
};

#endif//STRAITX_VALIDABLE_HPP