#ifndef STRAITX_OPENGL_GRAPHICS_PIPELINE_IMPL_HPP
#define STRAITX_OPENGL_GRAPHICS_PIPELINE_IMPL_HPP

#include "platform/opengl.hpp"
#include "graphics/api/graphics_pipeline.hpp"

namespace StraitX{
namespace GL{

struct Binding{
    ShaderBindingType Type;
    union{
        u32 UniformBuffer;
    }Data = {};

    constexpr Binding(ShaderBindingType type, u32 buffer);
};

struct DescriptorSet{
    PushArray<Binding,MaxShaderBindings> Bindings;

    void Bind()const;
};

struct GraphicsPipelineImpl: GraphicsPipeline{
    static GLenum s_BlendFunctionTable[];
    static GLenum s_BlendFactorTable[];
    static GLenum s_TopologyTable[];
    static GLenum s_RasterizationModeTable[];
    u32 VertexArray;
    u32 Program;
    bool Valid = true;
    PushArray<VertexAttribute, MaxVertexAttributes> Attributes;
    size_t AttributesStride = 0;

    GLenum Topology;
    //TODO last thing to match vulkan
    GLenum Rasterization;
    Viewport FramebufferViewport;
    GLenum BlendFunc;
    GLenum SrcBlendFactor;
    GLenum DstBlendFactor;

    DescriptorSet Set;

    GraphicsPipelineImpl(LogicalGPU &owner, const GraphicsPipelineProperties &props);

    virtual ~GraphicsPipelineImpl();

    virtual bool IsValid()override;

    void Bind()const;

    virtual void Bind(size_t index, const GPUBuffer &uniform_buffer)override;

    void BindVertexBuffer(u32 id)const;

    void BindIndexBuffer(u32 id)const;

    static GraphicsPipeline * NewImpl(LogicalGPU &owner, const GraphicsPipelineProperties &props);

    static void DeleteImpl(GraphicsPipeline *pipeline);

};

constexpr Binding::Binding(ShaderBindingType type, u32 buffer):
    Type(type)
{
    Data.UniformBuffer = buffer;
}

}//namespace GL::
}//namespace StraitX::

#endif//STRAITX_OPENGL_GRAPHICS_PIPELINE_IMPL_HPP