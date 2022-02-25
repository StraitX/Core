#ifndef STRAITX_VULKAN_GRAPHICS_PIPELINE_IMPL_HPP
#define STRAITX_VULKAN_GRAPHICS_PIPELINE_IMPL_HPP

#include "core/fixed_list.hpp"
#include "core/os/vulkan.hpp"
#include "graphics/api/graphics_pipeline.hpp"
#include "graphics/api/vulkan/pipeline.hpp"

namespace Vk{

VkFormat ToVkVertexAttributeFormat(VertexAttribute attribute);

VkPrimitiveTopology ToVkPrimitiveTopology(PrimitivesTopology topology);

VkPolygonMode ToVkPolygonMode(RasterizationMode mode);

VkBlendFactor ToVkBlendFactor(BlendFactor factor);

VkBlendOp ToVkBlendOp(BlendFunction func);

VkCompareOp ToVkDepthCompareOp(DepthFunction func);

class GraphicsPipelineImpl: public GraphicsPipeline, public Pipeline{
private:
    FixedList<VertexAttribute, MaxVertexAttributes> m_VertexAttributes;
public:
    GraphicsPipelineImpl(const GraphicsPipelineProperties &props);

    ~GraphicsPipelineImpl();

    ConstSpan<VertexAttribute> VertexAttributes()const override;
};

}//namespace Vk::

#endif//STRAITX_VULKAN_GRAPHICS_PIPELINE_IMPL_HPP