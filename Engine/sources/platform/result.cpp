#include "platform/result.hpp"
#include "platform/defs.hpp"


namespace StraitX{


const char *ResultNames[(int)Result::ResultCodesCount] = {
    S(Success),
    S(Failure),
    S(NotFound),
    S(InvalidArgs),
    S(NullObject),
    S(AlreadyDone),
    S(Unsupported),
    S(Overflow),
    S(Unavailable),
    S(PermissionDenied),
    S(AlreadyExist),
    S(IsDirectory),
    S(None),
    S(MemoryFailure)
};

}; // namespace StraitX::