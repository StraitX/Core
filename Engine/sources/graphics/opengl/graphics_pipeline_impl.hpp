#ifndef STRAITX_OPENGL_GRAPHICS_PIPELINE_IMPL_HPP
#define STRAITX_OPENGL_GRAPHICS_PIPELINE_IMPL_HPP

#include "platform/opengl.hpp"
#include "graphics/api/graphics_pipeline.hpp"
#include "graphics/opengl/descriptor_set_impl.hpp"

namespace StraitX{
namespace GL{

struct GraphicsPipelineImpl: GraphicsPipeline{
    static const GLenum s_BlendFunctionTable[];
    static const GLenum s_BlendFactorTable[];
	static const GLenum s_DepthFunctionTable[];
    static const GLenum s_TopologyTable[];
    static const GLenum s_RasterizationModeTable[];
    u32 VertexArray;
    u32 Program;
    bool Valid = true;
    PushArray<VertexAttribute, MaxVertexAttributes> Attributes;
    size_t AttributesStride = 0;

    GLenum PrimitiveTopology;
    GLenum RasterizationMode;
    GLenum BlendFunction;
	GLenum DepthFunction;
    GLenum SrcBlendFactor;
    GLenum DstBlendFactor;

	VirtualBinding VirtualBindings[DescriptorSetLayout::s_MaxBindings];

    GraphicsPipelineImpl(const GraphicsPipelineProperties &props);

    virtual ~GraphicsPipelineImpl();

    virtual bool IsValid()const override;

    void Bind()const;

    void BindVertexBuffer(u32 id)const;

    void BindIndexBuffer(u32 id)const;

    bool SupportsUniformBindings()const;

	bool SupportsVertexAttribFormat()const;

    static GraphicsPipeline * NewImpl(const GraphicsPipelineProperties &props);

    static void DeleteImpl(GraphicsPipeline *pipeline);

};

}//namespace GL::
}//namespace StraitX::

#endif//STRAITX_OPENGL_GRAPHICS_PIPELINE_IMPL_HPP