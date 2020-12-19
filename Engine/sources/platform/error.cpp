#include "platform/error.hpp"
#include "platform/defs.hpp"


namespace StraitX{


const char *ErrorNames[(int)Error::ErrorCodesCount] = {
    S(None),
    S(Success),
    S(Failure),
    S(NotFound),
    S(InvalidArgs),
    S(NullObject),
    S(AlreadyDone),
    S(Unsupported),
    S(Overflow),
    S(Unavailable)
};

}; // namespace StraitX::