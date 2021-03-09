#include "platform/window_system.hpp"

namespace StraitX{

Result WindowSystem::Initialize(){
    return Result::Success;
}

Result WindowSystem::Finalize(){
    return Result::Success;
}

Screen WindowSystem::MainScreen(){
    return {{}};
}

}//namespace StraitX::