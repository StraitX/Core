#include "platform/platform.hpp"
#include "platform/linux/display_x11.hpp"

namespace StraitX{

Error Platform::Initialize(){
    return Linux::DisplayX11::Instance().Open();
}


Error Platform::Finalize(){
    return Linux::DisplayX11::Instance().Close();
}

}; // namespace StratiX::