#include "core/log.hpp"
#include "graphics/api/gpu_context.hpp"
#include "graphics/api/graphics_api_loader.hpp"
#include "graphics/render_window.hpp"

namespace StraitX{
    
Result RenderWindow::Open(const Screen &screen, int width, int height, GraphicsAPI::API api, const SwapchainProperties &sw_props){
    auto res = Window::Open(screen, width, height);

    if(!res){
        LogError("Can't Open a window");
        return res;
    }

    m_LoaderResult = GraphicsAPILoader::Load(api);

    if(!m_LoaderResult)return m_LoaderResult;

    m_APIResult = GraphicsAPI::s_Instance->Initialize();

    if(!m_APIResult){
        LogError("Can't Initialize GraphicsAPI");
        return m_APIResult;
    }

    GPUContext::s_Instance = GPUContext::New();

    m_Swapchain = Swapchain::New(*this, sw_props);

    return res;
}

void RenderWindow::Close(){
    if(m_Swapchain)
        Swapchain::Delete(m_Swapchain);

    if(GPUContext::s_Instance)
        GPUContext::Delete(GPUContext::s_Instance);

    if(m_APIResult)
        GraphicsAPI::s_Instance->Finalize();

    Window::Close();
}

void RenderWindow::ClearColorBuffer(const Color &color){
    GPUContext::Get()->Begin();
    GPUContext::Get()->ClearFramebufferColorAttachments(CurrentFramebuffer(), Vector4f(color.R, color.G, color.B, color.A));
    GPUContext::Get()->End(); GPUContext::Get()->Submit();
}

void RenderWindow::ClearDepthBuffer(float value){
    GPUContext::Get()->Begin();
    GPUContext::Get()->ClearFramebufferDepthAttachments(CurrentFramebuffer(), value);
    GPUContext::Get()->End(); GPUContext::Get()->Submit();
}

void RenderWindow::SwapBuffers(){
    GPUContext::Get()->SwapFramebuffers(m_Swapchain);
}

}//namespace StraitX::