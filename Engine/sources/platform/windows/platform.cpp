#include "platform/platform.hpp"
#include "platform/windows/hinstance.hpp"
#include "platform/windows/window_win32.hpp"

namespace StraitX {

Result Platform::Initialize() {
    Windows::HInstance::Instance().Initialize();
    Windows::WindowWin32::RegClass();
    return Result::Success;
}


Result Platform::Finalize() {
    return Result::Success;
}

}; // namespace StratiX::