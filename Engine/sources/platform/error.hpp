#ifndef STRAITX_ERROR_HPP
#define STRAITX_ERROR_HPP

namespace StraitX{

using Error = int;

namespace ErrorCode{
    enum : int{
        None = 0,
        Success,
        Failure,
        NotFound,
        InvalidArgs,
        NoMatch,
        DoesNotExist,
        AlreadyDone
    };
}; // namespace Error::

}; // namespace StraitX::

#endif