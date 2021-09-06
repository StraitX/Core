#include "graphics/api/graphics_pipeline.hpp"
#include "graphics/api/graphics_api.hpp"
#include "core/os/vulkan.hpp"

#ifdef SX_VULKAN_SUPPORTED
    #include "graphics/api/vulkan/graphics_pipeline_impl.hpp"
#endif

size_t GetVertexAttributeSize(VertexAttribute attribute){
    static const u32 s_VertexAttributeSizeTable[]={
        1 * sizeof(float),
        2 * sizeof(float),
        3 * sizeof(float),
        4 * sizeof(float),

        1 * sizeof(u8),
        2 * sizeof(u8),
        3 * sizeof(u8),
        4 * sizeof(u8),

        1 * sizeof(s8),
        2 * sizeof(s8),
        3 * sizeof(s8),
        4 * sizeof(s8),

        1 * sizeof(u32),
        2 * sizeof(u32),
        3 * sizeof(u32),
        4 * sizeof(u32),

        1 * sizeof(s32),
        2 * sizeof(s32),
        3 * sizeof(s32),
        4 * sizeof(s32),
    };

    return s_VertexAttributeSizeTable[(size_t)attribute];
}


size_t CalculateStride(Span<VertexAttribute> attributes){
    size_t stride = 0;
    for(auto &attr: attributes){
        stride += GetVertexAttributeSize(attr);
    }
    return stride;
}


GraphicsPipeline *GraphicsPipeline::Create(const GraphicsPipelineProperties &props){
#ifdef SX_VULKAN_SUPPORTED
    if(GraphicsAPI::Backend() == GraphicsAPIBackend::Vulkan)
        return new Vk::GraphicsPipelineImpl(props);
#endif
    return nullptr;
}