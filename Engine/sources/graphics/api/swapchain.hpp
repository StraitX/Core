#ifndef STRAITX_SWAPCHAIN_HPP
#define STRAITX_SWAPCHAIN_HPP

#include "platform/window.hpp"
#include "core/noncopyable.hpp"
#include "graphics/api/render_pass.hpp"
#include "graphics/api/framebuffer.hpp"

namespace StraitX{

constexpr size_t MaxFramebuffers = 3;

struct SwapchainProperties{
    u32 FramebuffersCount = 2;
    SamplePoints FramebufferSamples = SamplePoints::Samples_1;
};

class GraphicsAPILoader;

class Swapchain: NonCopyable{
public:
    struct VTable{
        using NewProc    = Swapchain * (*)(const Window &window, const SwapchainProperties &props);
        using DeleteProc = void (*)(Swapchain *swapchain);

        NewProc    New    = nullptr;
        DeleteProc Delete = nullptr;
    };
private:
    friend class GraphicsAPILoader;

    static VTable s_VTable; 
public:

    virtual ~Swapchain() = default;

    virtual const RenderPass *FramebufferPass() = 0;

    virtual const Framebuffer *CurrentFramebuffer() = 0;

    sx_inline static Swapchain *New(const Window &window, const SwapchainProperties &props);

    sx_inline static void Delete(Swapchain *swapchain);
};

sx_inline Swapchain *Swapchain::New(const Window &window, const SwapchainProperties &props){
    return s_VTable.New(window, props);
}

sx_inline void Swapchain::Delete(Swapchain *swapchain){
    s_VTable.Delete(swapchain);
}

}//namespace StraitX::

#endif //STRAITX_SWAPCHAIN_HPP