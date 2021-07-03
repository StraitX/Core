#include "platform/result.hpp"
#include "platform/defs.hpp"

const char *Result::s_ResultNamesTable[(int)Result::ResultCodesCount] = {
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
    S(MemoryFailure),
    S(WrongFormat)
};