#include "graphics/api/graphics_api_vtable.hpp"

#include "graphics/opengl/graphics_context_impl.hpp"
#include "graphics/opengl/gpu_buffer_impl.hpp"
#include "graphics/opengl/cpu_buffer_impl.hpp"
#include "graphics/opengl/texture_impl.hpp"
#include "graphics/opengl/sampler_impl.hpp"
#include "graphics/opengl/shader_impl.hpp"
#include "graphics/opengl/render_pass_impl.hpp"
#include "graphics/opengl/descriptor_set_impl.hpp"
#include "graphics/opengl/graphics_pipeline_impl.hpp"
#include "graphics/opengl/framebuffer_impl.hpp"
#include "graphics/opengl/dma_impl.hpp"

GraphicsAPIVTable OpenGLVTable = {
     &GL::GraphicsContextImpl::s_Instance,
    {&GL::GPUBufferImpl::NewImpl, &GL::GPUBufferImpl::DeleteImpl},
    {&GL::CPUBufferImpl::NewImpl, &GL::CPUBufferImpl::DeleteImpl},
    {&GL::Texture2DImpl::NewImpl, &GL::Texture2DImpl::DeleteImpl},
    {&GL::SamplerImpl::NewImpl, &GL::SamplerImpl::DeleteImpl},
    {&GL::ShaderImpl::NewImpl, &GL::ShaderImpl::DeleteImpl},
    {&GL::RenderPassImpl::NewImpl, &GL::RenderPassImpl::DeleteImpl},
	{&GL::DescriptorSetLayoutImpl::NewImpl, &GL::DescriptorSetLayoutImpl::DeleteImpl},
	{&GL::DescriptorSetPoolImpl::NewImpl, &GL::DescriptorSetPoolImpl::DeleteImpl},
    {&GL::GraphicsPipelineImpl::NewImpl, &GL::GraphicsPipelineImpl::DeleteImpl},
    {&GL::FramebufferImpl::NewImpl, &GL::FramebufferImpl::DeleteImpl},
    {&GL::DMAImpl::CopyMem2GPUBufferImpl, &GL::DMAImpl::CopyCPU2GPUBufferImpl, &GL::DMAImpl::CopyCPU2GPUTextureImpl, &GL::DMAImpl::ChangeGPUTextureLayoutImpl}
};