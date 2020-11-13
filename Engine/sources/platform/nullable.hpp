#ifndef STRAITX_NULLABLE_HPP
#define STRAITX_NULLABLE_HPP

#include "platform/types.hpp"
#include "platform/compiler.hpp"

namespace StraitX{

class Nullable{
public:
    virtual bool operator==(nullptr_t ptr) = 0;

    sx_inline bool operator!=(nullptr_t ptr){
        return !(*this==ptr);
    }
};

}; //namespace StraitX::

#endif // STRAITX_NULLABLE_HPP