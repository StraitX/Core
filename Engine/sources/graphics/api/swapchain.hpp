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
    TextureFormat DepthFormat = TextureFormat::Unknown;
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

    Swapchain(const SwapchainProperties &props);

    virtual ~Swapchain() = default;

    virtual const Framebuffer *CurrentFramebuffer()const = 0;

    virtual const RenderPass *FramebufferPass()const = 0;

    static Swapchain *New(const Window &window, const SwapchainProperties &props);

    static void Delete(Swapchain *swapchain);
};

sx_inline Swapchain *Swapchain::New(const Window &window, const SwapchainProperties &props){
    return s_VTable.New(window, props);
}

sx_inline void Swapchain::Delete(Swapchain *swapchain){
    s_VTable.Delete(swapchain);
}

}//namespace StraitX::

#endif //STRAITX_SWAPCHAIN_HPP