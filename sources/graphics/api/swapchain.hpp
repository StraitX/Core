#ifndef STRAITX_SWAPCHAIN_HPP
#define STRAITX_SWAPCHAIN_HPP

#include "core/noncopyable.hpp"
#include "core/span.hpp"
#include "graphics/api/semaphore.hpp"
#include "graphics/api/fence.hpp"
#include "graphics/api/texture.hpp"
#include "core/os/window.hpp"


class Swapchain: public NonCopyable{
public:
    virtual ~Swapchain() = default;

    virtual void AcquireNext(const Semaphore &signal_semaphore, const Fence &signal_fence = Fence::Null) = 0;

    virtual void PresentCurrent(const Semaphore &wait_semaphore) = 0;

    //Span may be different after each PresentCurrent call
    virtual ConstSpan<Texture2D *> Images()const = 0;

    virtual u32 Current()const = 0;

    Texture2D *CurrentImage()const{
        return Images()[Current()];
    }

    TextureFormat Format()const{
        return CurrentImage()->Format();
    }

    Vector2u Size()const{
        return CurrentImage()->Size();
    }
public:
    static Swapchain *Create(const Window *window);
};

#endif//STRAITX_SWAPCHAIN_HPP