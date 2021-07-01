#ifndef STRAITX_GRAPHICS_API_VTABLE_HPP
#define STRAITX_GRAPHICS_API_VTABLE_HPP

#include "graphics/api/graphics_context.hpp"
#include "graphics/api/gpu_buffer.hpp"
#include "graphics/api/cpu_buffer.hpp"
#include "graphics/api/texture.hpp"
#include "graphics/api/sampler.hpp"
#include "graphics/api/shader.hpp"
#include "graphics/api/render_pass.hpp"
#include "graphics/api/descriptor_set.hpp"
#include "graphics/api/graphics_pipeline.hpp"
#include "graphics/api/framebuffer.hpp"
#include "graphics/api/dma.hpp"

namespace StraitX{

struct GraphicsAPIVTable{
    GraphicsContext *GraphicsContextPtr= {};

    GPUBuffer::VTable GPUBufferVTable = {};

    CPUBuffer::VTable CPUBufferVTable = {};

    Texture2D::VTable Texture2DVTable = {};

    Sampler::VTable SamplerVTable = {};

    Shader::VTable ShaderVTable = {};

    RenderPass::VTable RenderPassVTable = {};

	DescriptorSetLayout::VTable DescriptorSetLayoutVTable = {};

	DescriptorSetPool::VTable DescriptorPoolVTable = {};

    GraphicsPipeline::VTable GraphicsPipelineVTable = {};

    Framebuffer::VTable FramebufferVTable = {};

    DMA::VTable DMAVTable = {};
};

}//namespace StraitX::

#endif//STRAITX_GRAPHICS_API_VTABLE_HPP