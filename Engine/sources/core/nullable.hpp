#ifndef STRAITX_NULLABLE_HPP
#define STRAITX_NULLABLE_HPP

#include "platform/types.hpp"
#include "platform/compiler.hpp"

class Nullable{
public:
    virtual bool operator==(nullptr_t ptr) = 0;

    SX_INLINE bool operator!=(nullptr_t ptr){
        return !(*this==ptr);
    }

    SX_INLINE bool IsNull(){
        return *this==nullptr;
    }
};

#endif // STRAITX_NULLABLE_HPP