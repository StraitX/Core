#ifndef STRAITX_ERROR_HPP
#define STRAITX_ERROR_HPP

namespace StraitX{


enum class Result{
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

    ResultCodesCount
};

extern const char *ResultNames[(int)Result::ResultCodesCount];


}; // namespace StraitX::

#endif