#ifndef STRAITX_ERROR_HPP
#define STRAITX_ERROR_HPP

namespace StraitX{


enum class Error{
    None = 0,
    Success,
    Failure,
    NotFound,
    InvalidArgs,
    NullObject,
    AlreadyDone,
    Unsupported,
    Overflow,
    Unavailable,

    ErrorCodesCount
};

extern const char *ErrorNames[(int)Error::ErrorCodesCount];


}; // namespace StraitX::

#endif