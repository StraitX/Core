#ifndef STRAITX_OPENGL_GRAPHICS_PIPELINE_IMPL_HPP
#define STRAITX_OPENGL_GRAPHICS_PIPELINE_IMPL_HPP

#include "platform/opengl.hpp"
#include "graphics/api/graphics_pipeline.hpp"

namespace StraitX{
namespace GL{

struct GraphicsPipelineImpl: GraphicsPipeline{
    static GLenum s_BlendFunctionTable[];
    static GLenum s_BlendFactorTable[];
    static GLenum s_TopologyTable[];
    u32 VertexArray;
    u32 Program;
    bool Valid = true;
    PushArray<VertexAttribute, MaxVertexAttributes> Attributes;
    size_t AttributesStride = 0;

    GLenum Topology;
    //TODO last thing to match vulkan
    RasterizationMode Rasterization;
    Viewport FramebufferViewport;
    GLenum BlendFunc;
    GLenum SrcBlendFactor;
    GLenum DstBlendFactor;

    GraphicsPipelineImpl(LogicalGPU &owner, const GraphicsPipelineProperties &props);

    virtual ~GraphicsPipelineImpl();

    virtual bool IsValid()override;

    void Bind()const;

    void BindVertexBuffer(u32 id)const;

    void BindIndexBuffer(u32 id)const;

    static GraphicsPipeline * NewImpl(LogicalGPU &owner, const GraphicsPipelineProperties &props);

    static void DeleteImpl(GraphicsPipeline *pipeline);

};

}//namespace GL::
}//namespace StraitX::

#endif//STRAITX_OPENGL_GRAPHICS_PIPELINE_IMPL_HPP