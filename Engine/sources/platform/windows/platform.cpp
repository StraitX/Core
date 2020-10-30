#include "platform/platform.hpp"
#include "platform/windows/hinstance.hpp"

namespace StraitX {

Error Platform::Initialize() {
    Windows::HInstance::Instance().Initialize();
    return ErrorCode::Success;
}


Error Platform::Finalize() {
    return ErrorCode::Success;
}

}; // namespace StratiX::