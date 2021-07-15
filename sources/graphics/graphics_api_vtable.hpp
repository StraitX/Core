#ifndef STRAITX_GRAPHICS_API_VTABLE_HPP
#define STRAITX_GRAPHICS_API_VTABLE_HPP

#include "graphics/graphics_context.hpp"
#include "graphics/gpu_buffer.hpp"
#include "graphics/cpu_buffer.hpp"
#include "graphics/texture.hpp"
#include "graphics/sampler.hpp"
#include "graphics/shader.hpp"
#include "graphics/render_pass.hpp"
#include "graphics/descriptor_set.hpp"
#include "graphics/graphics_pipeline.hpp"
#include "graphics/framebuffer.hpp"
#include "graphics/dma.hpp"

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

#endif//STRAITX_GRAPHICS_API_VTABLE_HPP