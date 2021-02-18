#ifndef STRAITX_OPENGL_GRAPHICS_PIPELINE_IMPL_HPP
#define STRAITX_OPENGL_GRAPHICS_PIPELINE_IMPL_HPP

#include "platform/opengl.hpp"
#include "graphics/api/graphics_pipeline.hpp"

namespace StraitX{
namespace GL{

struct GraphicsPipelineImpl: GraphicsPipeline{
    static GLenum s_BlendFunctionTable[];
    static GLenum s_BlendFactorTable[];
    u32 VertexArray;
    u32 Program;
    bool Valid = true;
    PrimitivesTopology Topology;
    RasterizationMode Rasterization;
// TODO apply this while binding
    Viewport FramebufferViewport;
    BlendFunction BlendFunc;
    BlendFactor SrcBlendFactor;
    BlendFactor DstBlendFactor;

    GraphicsPipelineImpl(LogicalGPU &owner, const GraphicsPipelineProperties &props);

    virtual ~GraphicsPipelineImpl();

    virtual bool IsValid()override;

    void Bind()const;

    static GraphicsPipeline * NewImpl(LogicalGPU &owner, const GraphicsPipelineProperties &props);

    static void DeleteImpl(GraphicsPipeline *pipeline);

};

}//namespace GL::
}//namespace StraitX::

#endif//STRAITX_OPENGL_GRAPHICS_PIPELINE_IMPL_HPP