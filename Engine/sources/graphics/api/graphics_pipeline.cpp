#include "graphics_pipeline.hpp"

namespace StraitX{

GraphicsPipeline::VTable GraphicsPipeline::s_VTable;

u32 GraphicsPipeline::s_VertexAttributeSizeTable[]={
    1 * sizeof(i32),
    2 * sizeof(i32),
    3 * sizeof(i32),
    4 * sizeof(i32),

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
    bool has_vertex = false;
    bool has_fragment = false;
    //XXX

    for(auto *shader: props.Shaders){
        if(shader->GetType() == Shader::Vertex)
            has_vertex = true;
        if(shader->GetType() == Shader::Fragment)
            has_fragment = true;
    }

    CoreAssert(has_vertex && has_fragment, "GraphicsPipeline: Is not complete");
}

size_t GraphicsPipeline::CalculateStride(const ArrayPtr<const VertexAttribute> &attributes){
    size_t stride = 0;
    for(auto &attr: attributes){
        stride += s_VertexAttributeSizeTable[(size_t)attr];
    }
    return stride;
}


}//namespace StraitX::