#ifndef STRAITX_ERROR_HPP
#define STRAITX_ERROR_HPP

namespace StraitX{

#include "platform/compiler.hpp"

enum class Result{
    None    = -1,
    Success =  0,
    Failure,
    NotFound,
    InvalidArgs,
    NullObject,
    AlreadyDone,
    Unsupported,
    Overflow,
    Unavailable,
    PermissionDenied,
    AlreadyExist,
    IsDirectory,

    ResultCodesCount
};

sx_inline Result ResultError(bool is_error){
    return Result((int)is_error);
}

extern const char *ResultNames[(int)Result::ResultCodesCount + 1];


}; // namespace StraitX::

#endif