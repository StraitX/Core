#ifndef STRAITX_VULKAN_GRAPHICS_PIPELINE_IMPL_HPP
#define STRAITX_VULKAN_GRAPHICS_PIPELINE_IMPL_HPP

#include "platform/vulkan.hpp"
#include "graphics/api/graphics_pipeline.hpp"
#include "graphics/vulkan/render_pass_impl.hpp"

namespace Vk{

struct GraphicsPipelineImpl: GraphicsPipeline{
    static const VkFormat s_VertexAttributesTable[];
    static const VkPrimitiveTopology s_TopologyTable[];
    static const VkPolygonMode s_RasterizationModeTable[];
    static const VkBlendFactor s_BlendFactorTable[];
    static const VkBlendOp s_BlendFunctionTable[];
	static const VkCompareOp s_DepthFunctionTable[];

    VkPipeline Handle = VK_NULL_HANDLE;
    const Vk::RenderPassImpl *Pass;
    VkResult Status = VK_INCOMPLETE;

    VkPipelineLayout Layout = VK_NULL_HANDLE;

    GraphicsPipelineImpl(const GraphicsPipelineProperties &props);

    virtual ~GraphicsPipelineImpl();

    virtual bool IsValid()const override;

    static GraphicsPipeline * NewImpl(const GraphicsPipelineProperties &props);

    static void DeleteImpl(GraphicsPipeline *pipeline);

};

}//namespace Vk::

#endif//STRAITX_VULKAN_GRAPHICS_PIPELINE_IMPL_HPP