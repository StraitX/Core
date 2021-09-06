#ifndef STRAITX_VULKAN_GRAPHICS_PIPELINE_IMPL_HPP
#define STRAITX_VULKAN_GRAPHICS_PIPELINE_IMPL_HPP

#include "core/push_array.hpp"
#include "core/os/vulkan.hpp"
#include "graphics/api/graphics_pipeline.hpp"

namespace Vk{

VkFormat ToVkVertexAttributeFormat(VertexAttribute attribute);

VkPrimitiveTopology ToVkPrimitiveTopology(PrimitivesTopology topology);

VkPolygonMode ToVkPolygonMode(RasterizationMode mode);

VkBlendFactor ToVkBlendFactor(BlendFactor factor);

VkBlendOp ToVkBlendOp(BlendFunction func);

VkCompareOp ToVkDepthCompareOp(DepthFunction func);

class GraphicsPipelineImpl: public GraphicsPipeline{
private:
    VkPipeline m_Handle = VK_NULL_HANDLE;
    VkPipelineLayout m_Layout = VK_NULL_HANDLE;
    PushArray<VertexAttribute, MaxVertexAttributes> m_VertexAttributes;
public:
    GraphicsPipelineImpl(const GraphicsPipelineProperties &props);

    ~GraphicsPipelineImpl();

    ConstSpan<VertexAttribute> VertexAttributes()const override;

    operator VkPipeline()const{
        return m_Handle;
    }
};

}//namespace Vk::

#endif//STRAITX_VULKAN_GRAPHICS_PIPELINE_IMPL_HPP