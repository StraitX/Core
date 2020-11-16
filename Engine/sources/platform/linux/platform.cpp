#include "platform/platform.hpp"
#include "platform/linux/display_x11.hpp"

namespace StraitX{

Error Platform::Initialize(){
    return Error::Success;
}


Error Platform::Finalize(){
    return Error::Success;
}

}; // namespace StratiX::