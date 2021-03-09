#ifndef STRAITX_MACOS_SCREEN_IMPL_HPP
#define STRAITX_MACOS_SCREEN_IMPL_HPP

#include "platform/result.hpp"
#include "platform/events.hpp"

namespace StraitX{
namespace MacOS{

struct ScreenImpl{
    const Size2i &Size()const;

    const Size2f &DPI()const;
};

}//namespace MacOS::
}//namespace StraitX::

#endif//STRAITX_MACOS_SCREEN_IMPL_HPP