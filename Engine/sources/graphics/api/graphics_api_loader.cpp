#include "platform/opengl.hpp"
#include "platform/vulkan.hpp"
#include "core/log.hpp"
#include "graphics/api/graphics_api_loader.hpp"
#include "graphics/api/graphics_api_vtable.hpp"

namespace StraitX{

#ifdef SX_VULKAN_SUPPORTED
extern GraphicsAPIVtable VulkanVTable;
#endif

#ifdef SX_OPENGL_SUPPORTED
extern GraphicsAPIVtable OpenGLVTable;
#endif

GraphicsAPIVtable *GetVTable(GraphicsAPI::API api){
    switch (api){
        #ifdef SX_VULKAN_SUPPORTED
            case GraphicsAPI::Vulkan: return &VulkanVTable;
        #endif

        #ifdef SX_OPENGL_SUPPORTED
            case GraphicsAPI::OpenGL: return &OpenGLVTable;
        #endif
    default:
        return nullptr;
    }
}


Result GraphicsAPILoader::Load(GraphicsAPI::API api){
    GraphicsAPIVtable *api_vtable = GetVTable(api);
    if(!api_vtable){
        LogError("GraphicsAPILoader::Load: Unsupported API: %",GraphicsAPI::GetName(api));
        return Result::Unsupported;
    }

    GraphicsAPI::s_CurrentAPI = api;

    GraphicsAPI::s_Instance = api_vtable->GraphicsAPIPtr;

    LogicalGPU::s_Instance = api_vtable->LogicalGPUPtr;

    GPUContext::s_VTable = api_vtable->GPUContextVTable;

    Swapchain::s_VTable = api_vtable->SwapchainVTable;

    GPUBuffer::s_VTable = api_vtable->GPUBufferVTable;

    CPUBuffer::s_VTable = api_vtable->CPUBufferVTable;

    GPUTexture::s_VTable = api_vtable->GPUTextureVTable;

    Shader::s_VTable = api_vtable->ShaderVTable;

    RenderPass::s_VTable = api_vtable->RenderPassVTable;

    GraphicsPipeline::s_VTable = api_vtable->GraphicsPipelineVTable;
    
    Framebuffer::s_VTable = api_vtable->FramebufferVTable;

    return Result::Success;
}

}//namespace StraitX::