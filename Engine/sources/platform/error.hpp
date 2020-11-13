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
        NullObject,
        AlreadyDone,
        NotSupported,

        ErrorCodesCount
    };
}; // namespace Error::

}; // namespace StraitX::

#endif