#include "graphics/api/graphics_api_vtable.hpp"

#include "graphics/opengl/graphics_api_impl.hpp"
#include "graphics/opengl/logical_gpu_impl.hpp"
#include "graphics/opengl/gpu_context_impl.hpp"
#include "graphics/opengl/swapchain_impl.hpp"
#include "graphics/opengl/gpu_buffer_impl.hpp"
#include "graphics/opengl/cpu_buffer_impl.hpp"
#include "graphics/opengl/gpu_texture_impl.hpp"
#include "graphics/opengl/sampler_impl.hpp"
#include "graphics/opengl/shader_impl.hpp"
#include "graphics/opengl/render_pass_impl.hpp"
#include "graphics/opengl/graphics_pipeline_impl.hpp"
#include "graphics/opengl/framebuffer_impl.hpp"

namespace StraitX{

GraphicsAPIVtable OpenGLVTable = {
    &GL::GraphicsAPIImpl::Instance,
    &GL::LogicalGPUImpl::Instance,
    {&GL::GPUContextImpl::NewImpl, &GL::GPUContextImpl::DeleteImpl},
    {&GL::SwapchainImpl::NewImpl, &GL::SwapchainImpl::DeleteImpl},
    {&GL::GPUBufferImpl::NewImpl, &GL::GPUBufferImpl::DeleteImpl},
    {&GL::CPUBufferImpl::NewImpl, &GL::CPUBufferImpl::DeleteImpl},
    {&GL::GPUTextureImpl::NewImpl, &GL::GPUTextureImpl::DeleteImpl},
    {&GL::SamplerImpl::NewImpl, &GL::SamplerImpl::DeleteImpl},
    {&GL::ShaderImpl::NewImpl, &GL::ShaderImpl::DeleteImpl},
    {&GL::RenderPassImpl::NewImpl, &GL::RenderPassImpl::DeleteImpl},
    {&GL::GraphicsPipelineImpl::NewImpl, &GL::GraphicsPipelineImpl::DeleteImpl},
    {&GL::FramebufferImpl::NewImpl, &GL::FramebufferImpl::DeleteImpl}
};

}//namespace StraitX::