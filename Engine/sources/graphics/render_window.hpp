#ifndef STRAITX_RENDER_WINDOW_HPP
#define STRAITX_RENDER_WINDOW_HPP

#include "platform/window.hpp"
#include "graphics/color.hpp"
#include "graphics/api/swapchain.hpp"
#include "graphics/api/graphics_api.hpp"

namespace StraitX{
    
class RenderWindow: public Window{
private:
    Swapchain *m_Swapchain = nullptr;
    Result m_LoaderResult = Result::Failure;
    Result m_APIResult = Result::Failure;
public:
    Result Open(const Screen &screen, int width, int height, GraphicsAPI::API api, const SwapchainProperties &sw_props);

    void Close();

    void ClearColorBuffer(const Color &color = Color::Black);

    void ClearDepthBuffer(float value = 1.0);

    void SwapBuffers();

    const Swapchain *GetSwapchain()const{
        return m_Swapchain;
    }

    const RenderPass *Pass()const{
        return m_Swapchain->FramebufferPass();
    }

    const Framebuffer *CurrentFramebuffer()const{
        return m_Swapchain->CurrentFramebuffer();
    }
};

}//namespace StraitX::

#endif//STRAITX_RENDER_WINDOW_HPP