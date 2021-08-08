#ifndef STRAITX_LINUX_SCREEN_IMPL_HPP
#define STRAITX_LINUX_SCREEN_IMPL_HPP

#include "core/types.hpp"
#include "core/math/vector2.hpp"

namespace Linux{

struct ScreenImpl{
    void *Handle = nullptr;
    unsigned int Index = -1;
    Vector2s Size = {};
    Vector2f DPI = {};

	ScreenImpl() = default;

	ScreenImpl(void *screen_handle, unsigned int screen_index);
};

} // namespace Linux::


#endif // STRAITX_LINUX_SCREEN_IMPL_HPP