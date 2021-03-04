#ifndef STRAITX_VULKAN_GRAPHICS_PIPELINE_IMPL_HPP
#define STRAITX_VULKAN_GRAPHICS_PIPELINE_IMPL_HPP

#include "platform/vulkan.hpp"
#include "graphics/api/graphics_pipeline.hpp"
#include "graphics/vulkan/logical_gpu_impl.hpp"
#include "graphics/vulkan/render_pass_impl.hpp"

namespace StraitX{
namespace Vk{

struct GraphicsPipelineImpl: GraphicsPipeline{
    static VkFormat s_VertexAttributesTable[];
    static VkPrimitiveTopology s_TopologyTable[];
    static VkPolygonMode s_RasterizationModeTable[];
    static VkBlendFactor s_BlendFactorTable[];
    static VkBlendOp s_BlendFunctionTable[];
    static VkDescriptorType s_DescriptorTypeTable[];

    Vk::LogicalGPUImpl *Owner = nullptr;
    VkPipeline Handle = VK_NULL_HANDLE;
    const Vk::RenderPassImpl *Pass;
    VkResult Status = VK_INCOMPLETE;
    VkRect2D Scissors = {};

    VkPipelineLayout Layout = VK_NULL_HANDLE;

    VkDescriptorPool Pool = VK_NULL_HANDLE;
    VkDescriptorSetLayout SetLayout = VK_NULL_HANDLE;
    VkDescriptorSet Set = VK_NULL_HANDLE;

    GraphicsPipelineImpl(LogicalGPU &owner, const GraphicsPipelineProperties &props);

    virtual ~GraphicsPipelineImpl();

    virtual bool IsValid()override;

    virtual void Bind(size_t index, const GPUBuffer &uniform_buffer)override;

    static GraphicsPipeline * NewImpl(LogicalGPU &owner, const GraphicsPipelineProperties &props);

    static void DeleteImpl(GraphicsPipeline *pipeline);

};

}//namespace Vk::
}//namespace StraitX::

#endif//STRAITX_VULKAN_GRAPHICS_PIPELINE_IMPL_HPP