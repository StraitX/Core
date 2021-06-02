#ifndef STRAITX_ERROR_HPP
#define STRAITX_ERROR_HPP

#include "platform/types.hpp"

namespace StraitX{

class Result{
public:
    enum Type: u8{
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
        None,
        MemoryFailure,
        WrongFormat,

        ResultCodesCount
    };
private:
    static const char *s_ResultNamesTable[(size_t)Result::ResultCodesCount];
    u8 m_Value;
public:
    constexpr Result(Type result);

    constexpr explicit Result(bool result);

    constexpr explicit operator bool()const;

    constexpr bool operator!()const;

    friend constexpr bool operator==(Result l, Result r);

    friend constexpr bool operator!=(Result l, Result r);

    const char *Name()const;
};

constexpr Result::Result(Type result):
    m_Value(result)
{}

constexpr Result::Result(bool result):
    Result((Type)!result)
{}

constexpr Result::operator bool()const{
    return !bool(m_Value);
}

constexpr bool Result::operator!()const{
    return bool(m_Value);
}
constexpr bool operator==(Result l, Result r){
    return l.m_Value == r.m_Value;
}

constexpr bool operator!=(Result l, Result r){
    return !(l == r);
}

SX_INLINE const char *Result::Name()const{
    return s_ResultNamesTable[m_Value];
}

constexpr Result ResultError(bool is_error){
    return Result(!is_error);
}

}; // namespace StraitX::

#endif