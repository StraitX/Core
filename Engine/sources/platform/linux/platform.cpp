#include "platform/platform.hpp"
#include "platform/linux/display.hpp"

namespace StraitX{

Error Platform::Initialize(){
    return Linux::Display::Instance().Open();
}


Error Platform::Finalize(){
    return Linux::Display::Instance().Close();
}

}; // namespace StratiX::