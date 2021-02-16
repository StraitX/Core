#include "core/log.hpp"
#include "graphics/api/graphics_api_loader.hpp"

#include "graphics/api/graphics_api.hpp"
#include "graphics/vulkan/graphics_api_impl.hpp"
#include "graphics/opengl/graphics_api_impl.hpp"

#include "graphics/api/logical_gpu.hpp"
#include "graphics/vulkan/logical_gpu_impl.hpp"
#include "graphics/opengl/logical_gpu_impl.hpp"

#include "graphics/api/gpu_context.hpp"
#include "graphics/vulkan/gpu_context_impl.hpp"
#include "graphics/opengl/gpu_context_impl.hpp"

#include "graphics/api/swapchain.hpp"
#include "graphics/vulkan/swapchain_impl.hpp"
#include "graphics/opengl/swapchain_impl.hpp"

#include "graphics/api/gpu_buffer.hpp"
#include "graphics/vulkan/gpu_buffer_impl.hpp"
#include "graphics/opengl/gpu_buffer_impl.hpp"

#include "graphics/api/cpu_buffer.hpp"
#include "graphics/vulkan/cpu_buffer_impl.hpp"
#include "graphics/opengl/cpu_buffer_impl.hpp"

#include "graphics/api/gpu_texture.hpp"
#include "graphics/vulkan/gpu_texture_impl.hpp"
#include "graphics/opengl/gpu_texture_impl.hpp"

#include "graphics/api/shader.hpp"
#include "graphics/vulkan/shader_impl.hpp"
#include "graphics/opengl/shader_impl.hpp"

#include "graphics/api/render_pass.hpp"
#include "graphics/vulkan/render_pass_impl.hpp"
#include "graphics/opengl/render_pass_impl.hpp"

#include "graphics/api/graphics_pipeline.hpp"
#include "graphics/vulkan/graphics_pipeline_impl.hpp"
#include "graphics/opengl/graphics_pipeline_impl.hpp"

namespace StraitX{

static GraphicsAPI *const GraphicsAPITable[GraphicsAPI::ElementsCount] = {
    nullptr,
    &Vk::GraphicsAPIImpl::Instance,
    &GL::GraphicsAPIImpl::Instance
};

static LogicalGPU *const LogicalGPUTable[GraphicsAPI::ElementsCount] = {
    nullptr,
    &Vk::LogicalGPUImpl::Instance,
    &GL::LogicalGPUImpl::Instance
};

static GPUContext::VTable GPUContextTable[GraphicsAPI::ElementsCount] = {
    {nullptr, nullptr},
    {&Vk::GPUContextImpl::NewImpl, &Vk::GPUContextImpl::DeleteImpl},
    {&GL::GPUContextImpl::NewImpl, &GL::GPUContextImpl::DeleteImpl}
};

static Swapchain::VTable SwapchainTable[GraphicsAPI::ElementsCount] = {
    {nullptr, nullptr},
    {&Vk::SwapchainImpl::NewImpl, &Vk::SwapchainImpl::DeleteImpl},
    {&GL::SwapchainImpl::NewImpl, &GL::SwapchainImpl::DeleteImpl}
};

static GPUBuffer::VTable GPUBufferTable[GraphicsAPI::ElementsCount] = {
    {nullptr, nullptr},
    {&Vk::GPUBufferImpl::NewImpl, &Vk::GPUBufferImpl::DeleteImpl},
    {&GL::GPUBufferImpl::NewImpl, &GL::GPUBufferImpl::DeleteImpl}
};

static CPUBuffer::VTable CPUBufferTable[GraphicsAPI::ElementsCount] = {
    {nullptr, nullptr},
    {&Vk::CPUBufferImpl::NewImpl, &Vk::CPUBufferImpl::DeleteImpl},
    {&GL::CPUBufferImpl::NewImpl, &GL::CPUBufferImpl::DeleteImpl}
};

static GPUTexture::VTable GPUTextureTable[GraphicsAPI::ElementsCount] = {
    {nullptr, nullptr},
    {&Vk::GPUTextureImpl::NewImpl, &Vk::GPUTextureImpl::DeleteImpl},
    {&GL::GPUTextureImpl::NewImpl, &GL::GPUTextureImpl::DeleteImpl}
};

static Shader::VTable ShaderTable[GraphicsAPI::ElementsCount] = {
    {nullptr, nullptr},
    {&Vk::ShaderImpl::NewImpl, &Vk::ShaderImpl::DeleteImpl},
    {&GL::ShaderImpl::NewImpl, &GL::ShaderImpl::DeleteImpl}
};

static RenderPass::VTable RenderPassTable[GraphicsAPI::ElementsCount] = {
    {nullptr, nullptr},
    {&Vk::RenderPassImpl::NewImpl, &Vk::RenderPassImpl::DeleteImpl},
    {&GL::RenderPassImpl::NewImpl, &GL::RenderPassImpl::DeleteImpl}
};

static GraphicsPipeline::VTable GraphicsPipelineTable[GraphicsAPI::ElementsCount] = {
    {nullptr, nullptr},
    {&Vk::GraphicsPipelineImpl::NewImpl, &Vk::GraphicsPipelineImpl::DeleteImpl},
    {&GL::GraphicsPipelineImpl::NewImpl, &GL::GraphicsPipelineImpl::DeleteImpl}
};

Result GraphicsAPILoader::Load(GraphicsAPI::API api){
    if(api != GraphicsAPI::OpenGL && api != GraphicsAPI::Vulkan){
        LogError("GraphicsAPILoader::Load: Unknown API: %",(int)api);
        return Result::Unsupported;
    }
    GraphicsAPI::s_CurrentAPI = api;

    GraphicsAPI::s_Instance = GraphicsAPITable[api];

    LogicalGPU::s_Instance = LogicalGPUTable[api];

    GPUContext::s_VTable = GPUContextTable[api];

    Swapchain::s_VTable = SwapchainTable[api];

    GPUBuffer::s_VTable = GPUBufferTable[api];

    CPUBuffer::s_VTable = CPUBufferTable[api];

    GPUTexture::s_VTable = GPUTextureTable[api];

    Shader::s_VTable = ShaderTable[api];

    RenderPass::s_VTable = RenderPassTable[api];

    GraphicsPipeline::s_VTable = GraphicsPipelineTable[api];

    return Result::Success;
}

}//namespace StraitX::