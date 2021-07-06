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

enum class VertexAttribute{
	Float32x1,
	Float32x2,
	Float32x3,
	Float32x4,

	UNorm8x1,
	UNorm8x2,
	UNorm8x3,
	UNorm8x4,

	SNorm8x1,
	SNorm8x2,
	SNorm8x3,
	SNorm8x4,

	Uint32x1,
	Uint32x2,
	Uint32x3,
	Uint32x4,

	Sint32x1,
	Sint32x2,
	Sint32x3,
	Sint32x4,
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

enum class DepthFunction{
	Always = 0,
	Less,
	LessOrEqual,
	Equal,
	NotEqual,
	GreatedOrEqual,
	Greater,
	Never
};

struct GraphicsPipelineProperties{
    Span<const Shader*> Shaders;
    Span<VertexAttribute> VertexAttributes;
    enum PrimitivesTopology PrimitivesTopology;
    enum RasterizationMode RasterizationMode;
	enum DepthFunction DepthFunction;
    enum BlendFunction BlendFunction;
    BlendFactor SrcBlendFactor;
    BlendFactor DstBlendFactor;
    const RenderPass *Pass;
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

	static const u32 s_VertexAttributeSizeTable[];
	
    friend class GraphicsAPILoader;	
protected:
	static u32 AttributeSize(VertexAttribute attribute);
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

#endif//STRAITX_GRAPHICS_PIPELINE_HPP