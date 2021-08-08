#include "core/os/opengl.hpp"
#include "core/os/vulkan.hpp"
#include "core/log.hpp"
#include "graphics/graphics_api_loader.hpp"
#include "graphics/graphics_api_vtable.hpp"

#ifdef SX_VULKAN_SUPPORTED
extern GraphicsAPIVTable VulkanVTable;
#endif

#ifdef SX_OPENGL_SUPPORTED
extern GraphicsAPIVTable OpenGLVTable;
#endif

GraphicsAPIVTable *GetVTable(GraphicsAPI api){
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


Result GraphicsAPILoader::Load(GraphicsAPI api){
    GraphicsAPIVTable *api_vtable = GetVTable(api);
    if(!api_vtable){
        LogError("GraphicsAPILoader::Load: Unsupported API: %", api.Name());
        return Result::Unsupported;
    }

	GraphicsContext::s_CurrentAPI = api;
	
	GraphicsContext::s_Instance = api_vtable->GraphicsContextPtr;

    GPUBuffer::s_VTable = api_vtable->GPUBufferVTable;

    CPUBuffer::s_VTable = api_vtable->CPUBufferVTable;

    Texture2D::s_VTable = api_vtable->Texture2DVTable;

    Sampler::s_VTable = api_vtable->SamplerVTable;

    Shader::s_VTable = api_vtable->ShaderVTable;

    RenderPass::s_VTable = api_vtable->RenderPassVTable;

	DescriptorSetLayout::s_VTable = api_vtable->DescriptorSetLayoutVTable;

	DescriptorSetPool::s_VTable = api_vtable->DescriptorPoolVTable;

    GraphicsPipeline::s_VTable = api_vtable->GraphicsPipelineVTable;
    
    Framebuffer::s_VTable = api_vtable->FramebufferVTable;

    DMA::s_VTable = api_vtable->DMAVTable;

    return Result::Success;
}