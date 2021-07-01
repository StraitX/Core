#ifndef STRAITX_GRAPHICS_PIPELINE_HPP
#define STRAITX_GRAPHICS_PIPELINE_HPP

#include "core/span.hpp"
#include "core/noncopyable.hpp"
#include "core/validable.hpp"
#include "graphics/api/shader.hpp"
#include "graphics/api/gpu_buffer.hpp"
#include "graphics/api/render_pass.hpp"
#include "graphics/api/texture.hpp"
#include "graphics/api/sampler.hpp"
#include "graphics/api/descriptor_set.hpp"

namespace StraitX{


enum class VertexAttribute{
    Int = 0,
    Int2,
    Int3,
    Int4,

    Uint,
    Uint2,
    Uint3,
    Uint4,

    Float,
    Float2,
    Float3,
    Float4
};

//TODO: Take value from docs
constexpr size_t MaxShaderBindings = 14;

enum class PrimitivesTopology{
    Points = 0,
    Lines,
    LinesStrip,
    Triangles,
    TrianglesStrip
};

enum class RasterizationMode{
    Fill = 0,
    Lines,
    Points
};

enum class BlendFunction{
    Add,
};

enum class BlendFactor{
    Zero,
    One,
    OneMinusSrcAlpha,
    OneMinusDstAlpha,
    SrcAlpha,
    DstAlpha 
};

struct Viewport{
    s32 x;
    s32 y;
    u32 Width;
    u32 Height;
};

struct GraphicsPipelineProperties{
    Span<const Shader*> Shaders;
    Span<VertexAttribute> VertexAttributes;
    PrimitivesTopology Topology;
    Viewport FramebufferViewport;
    RasterizationMode Rasterization;
    BlendFunction BlendFunc;
    BlendFactor SrcBlendFactor;
    BlendFactor DstBlendFactor;
    const RenderPass *Pass;
    //Span<ShaderBinding> ShaderBindings;
	const class DescriptorSetLayout *DescriptorSetLayout;
};
//OpenGL said that
constexpr size_t MaxVertexAttributes = 8;

class GraphicsAPILoader;

class GraphicsPipeline: public Validable, public NonCopyable{
public:
    struct VTable{
        using NewProc    = GraphicsPipeline * (*)(const GraphicsPipelineProperties &props);
        using DeleteProc = void (*)(GraphicsPipeline *);

        NewProc    New    = nullptr;
        DeleteProc Delete = nullptr;
    };
private:
    static VTable s_VTable;

    friend class GraphicsAPILoader;
protected:
    static u32 s_VertexAttributeSizeTable[];
public:
    GraphicsPipeline(const GraphicsPipelineProperties &props);

    virtual ~GraphicsPipeline() = default;

    static size_t CalculateStride(Span<VertexAttribute> attributes);
    
    static GraphicsPipeline *New(const GraphicsPipelineProperties &props);

    static void Delete(GraphicsPipeline *pipeline);

};

SX_INLINE GraphicsPipeline *GraphicsPipeline::New(const GraphicsPipelineProperties &props){
    return s_VTable.New(props);
}

SX_INLINE void GraphicsPipeline::Delete(GraphicsPipeline *pipeline){
    s_VTable.Delete(pipeline);
}

}//namespace StraitX::

#endif//STRAITX_GRAPHICS_PIPELINE_HPP