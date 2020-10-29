#include "platform/platform.hpp"
#include "platform/linux/display.hpp"

namespace StraitX{

Error Platform::Initialize(){
    Linux::Display::Instance().Open();

}


Error Platform::Finalize(){

    Linux::Display::Instance().Close();
}

}; // namespace StratiX::