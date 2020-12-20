#include "platform/result.hpp"
#include "platform/defs.hpp"


namespace StraitX{


const char *ResultNames[(int)Result::ResultCodesCount] = {
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