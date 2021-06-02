#include "platform/compiler.hpp"
#include "graphics_pipeline.hpp"

namespace StraitX{

GraphicsPipeline::VTable GraphicsPipeline::s_VTable;

u32 GraphicsPipeline::s_VertexAttributeSizeTable[]={
    1 * sizeof(s32),
    2 * sizeof(s32),
    3 * sizeof(s32),
    4 * sizeof(s32),

    1 * sizeof(u32),
    2 * sizeof(u32),
    3 * sizeof(u32),
    4 * sizeof(u32),

    1 * sizeof(float),
    2 * sizeof(float),
    3 * sizeof(float),
    4 * sizeof(float)
};

GraphicsPipeline::GraphicsPipeline(const GraphicsPipelineProperties &props){
#ifdef SX_DEBUG
    bool has_vertex = false;
    bool has_fragment = false;

    for(auto *shader: props.Shaders){
        if(shader->GetType() == Shader::Vertex)
            has_vertex = true;
        if(shader->GetType() == Shader::Fragment)
            has_fragment = true;
    }
    SX_CORE_ASSERT(has_vertex && has_fragment, "GraphicsPipeline: Is not complete");
#else
    (void)props;
#endif
}

size_t GraphicsPipeline::CalculateStride(Span<VertexAttribute> attributes){
    size_t stride = 0;
    for(auto &attr: attributes){
        stride += s_VertexAttributeSizeTable[(size_t)attr];
    }
    return stride;
}


}//namespace StraitX::