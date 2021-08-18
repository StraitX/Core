#ifndef STRAITX_SWAPCHAIN_HPP
#define STRAITX_SWAPCHAIN_HPP

#include "core/noncopyable.hpp"
#include "core/span.hpp"
#include "graphics/api/semaphore.hpp"
#include "graphics/api/texture.hpp"
#include "core/os/window.hpp"


class Swapchain: public NonCopyable{
public:
    virtual void AcquireNext(const Semaphore &signal_semaphore) = 0;

    virtual void PresentCurrent(const Semaphore &wait_semaphore) = 0;

    //Span may be different after each PresentCurrent call
    virtual Span<const Texture2D *> Images()const = 0;

    virtual u32 Current()const = 0;
public:
    static Swapchain *Create(const Window *window);
};

#endif//STRAITX_SWAPCHAIN_HPP