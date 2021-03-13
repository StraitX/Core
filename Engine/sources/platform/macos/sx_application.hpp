#ifndef STRAITX_MACOS_SX_APPLICATION_HPP
#define STRAITX_MACOS_SX_APPLICATION_HPP

#include "platform/keyboard.hpp"
#include "platform/compiler.hpp"

namespace StraitX{
namespace MacOS{

struct SXApplication{
    static bool s_KeyboardState[Keyboard::KeyCount];

    static void ProcessEvents();
};


}//namespace MacOS::
}//namespace StraitX::

#endif//STRAITX_MACOS_SX_APPLICATION_HPP