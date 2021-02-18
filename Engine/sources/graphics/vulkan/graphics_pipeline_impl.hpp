#ifndef STRAITX_VULKAN_GRAPHICS_PIPELINE_IMPL_HPP
#define STRAITX_VULKAN_GRAPHICS_PIPELINE_IMPL_HPP

#include "platform/vulkan.hpp"
#include "graphics/api/graphics_pipeline.hpp"
#include "graphics/vulkan/logical_gpu_impl.hpp"

namespace StraitX{
namespace Vk{

struct GraphicsPipelineImpl: GraphicsPipeline{
    static VkFormat s_VertexAttributesTable[];
    static VkPrimitiveTopology s_TopologyTable[];
    static VkPolygonMode s_RasterizationModeTable[];
    static VkBlendFactor s_BlendFactorTable[];
    static VkBlendOp s_BlendFunctionTable[];

    Vk::LogicalGPUImpl *Owner = nullptr;
    VkPipeline Handle = VK_NULL_HANDLE;
    VkResult Status = VK_INCOMPLETE;
    VkRect2D Scissors = {};

    GraphicsPipelineImpl(LogicalGPU &owner, const GraphicsPipelineProperties &props);

    virtual ~GraphicsPipelineImpl();

    virtual bool IsValid()override;

    static GraphicsPipeline * NewImpl(LogicalGPU &owner, const GraphicsPipelineProperties &props);

    static void DeleteImpl(GraphicsPipeline *pipeline);

};

}//namespace Vk::
}//namespace StraitX::

#endif//STRAITX_VULKAN_GRAPHICS_PIPELINE_IMPL_HPP