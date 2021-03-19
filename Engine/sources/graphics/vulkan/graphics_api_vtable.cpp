#include "graphics/api/graphics_api_vtable.hpp"

#include "graphics/vulkan/graphics_api_impl.hpp"
#include "graphics/vulkan/logical_gpu_impl.hpp"
#include "graphics/vulkan/gpu_context_impl.hpp"
#include "graphics/vulkan/swapchain_impl.hpp"
#include "graphics/vulkan/gpu_buffer_impl.hpp"
#include "graphics/vulkan/cpu_buffer_impl.hpp"
#include "graphics/vulkan/gpu_texture_impl.hpp"
#include "graphics/vulkan/sampler_impl.hpp"
#include "graphics/vulkan/shader_impl.hpp"
#include "graphics/vulkan/render_pass_impl.hpp"
#include "graphics/vulkan/graphics_pipeline_impl.hpp"
#include "graphics/vulkan/framebuffer_impl.hpp"
#include "graphics/vulkan/dma_impl.hpp"

namespace StraitX{

GraphicsAPIVtable VulkanVTable = {
    &Vk::GraphicsAPIImpl::Instance,
    &Vk::LogicalGPUImpl::Instance,
    {&Vk::GPUContextImpl::NewImpl, &Vk::GPUContextImpl::DeleteImpl},
    {&Vk::SwapchainImpl::NewImpl, &Vk::SwapchainImpl::DeleteImpl},
    {&Vk::GPUBufferImpl::NewImpl, &Vk::GPUBufferImpl::DeleteImpl},
    {&Vk::CPUBufferImpl::NewImpl, &Vk::CPUBufferImpl::DeleteImpl},
    {&Vk::GPUTextureImpl::NewImpl, &Vk::GPUTextureImpl::DeleteImpl},
    {&Vk::SamplerImpl::NewImpl, &Vk::SamplerImpl::DeleteImpl},
    {&Vk::ShaderImpl::NewImpl, &Vk::ShaderImpl::DeleteImpl},
    {&Vk::RenderPassImpl::NewImpl, &Vk::RenderPassImpl::DeleteImpl},
    {&Vk::GraphicsPipelineImpl::NewImpl, &Vk::GraphicsPipelineImpl::DeleteImpl},
    {&Vk::FramebufferImpl::NewImpl, &Vk::FramebufferImpl::DeleteImpl},
    {&Vk::DMAImpl::CopyCPU2GPUBufferImpl, &Vk::DMAImpl::CopyCPU2GPUTextureImpl}
};

}//namespace StraitX::