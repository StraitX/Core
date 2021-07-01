#include "graphics/api/graphics_api_vtable.hpp"

#include "graphics/vulkan/graphics_context_impl.hpp"
#include "graphics/vulkan/gpu_buffer_impl.hpp"
#include "graphics/vulkan/cpu_buffer_impl.hpp"
#include "graphics/vulkan/texture_impl.hpp"
#include "graphics/vulkan/sampler_impl.hpp"
#include "graphics/vulkan/shader_impl.hpp"
#include "graphics/vulkan/render_pass_impl.hpp"
#include "graphics/vulkan/descriptor_set_impl.hpp"
#include "graphics/vulkan/graphics_pipeline_impl.hpp"
#include "graphics/vulkan/framebuffer_impl.hpp"
#include "graphics/vulkan/dma_impl.hpp"

namespace StraitX{

GraphicsAPIVTable VulkanVTable = {
     &Vk::GraphicsContextImpl::s_Instance,
    {&Vk::GPUBufferImpl::NewImpl, &Vk::GPUBufferImpl::DeleteImpl},
    {&Vk::CPUBufferImpl::NewImpl, &Vk::CPUBufferImpl::DeleteImpl},
    {&Vk::Texture2DImpl::NewImpl, &Vk::Texture2DImpl::DeleteImpl},
    {&Vk::SamplerImpl::NewImpl, &Vk::SamplerImpl::DeleteImpl},
    {&Vk::ShaderImpl::NewImpl, &Vk::ShaderImpl::DeleteImpl},
    {&Vk::RenderPassImpl::NewImpl, &Vk::RenderPassImpl::DeleteImpl},
	{&Vk::DescriptorSetLayoutImpl::NewImpl, &Vk::DescriptorSetLayoutImpl::DeleteImpl},
	{&Vk::DescriptorSetPoolImpl::NewImpl, &Vk::DescriptorSetPoolImpl::DeleteImpl},
    {&Vk::GraphicsPipelineImpl::NewImpl, &Vk::GraphicsPipelineImpl::DeleteImpl},
    {&Vk::FramebufferImpl::NewImpl, &Vk::FramebufferImpl::DeleteImpl},
    {&Vk::DMAImpl::CopyMem2GPUBufferImpl, &Vk::DMAImpl::CopyCPU2GPUBufferImpl, &Vk::DMAImpl::CopyCPU2GPUTextureImpl, &Vk::DMAImpl::ChangeGPUTextureLayoutImpl}
};

}//namespace StraitX::