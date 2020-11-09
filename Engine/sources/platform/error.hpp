#ifndef STRAITX_ERROR_HPP
#define STRAITX_ERROR_HPP

namespace StraitX{

using Error = int;

namespace ErrorCode{
    enum : int{
        Success = 0,
        Failure,
        NotFound,
        WrongArgs,
        NoMatch
    };
}; // namespace Error::

}; // namespace StraitX::

#endif