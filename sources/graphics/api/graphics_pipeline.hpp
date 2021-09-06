#ifndef STRAITX_GRAPHICS_PIPELINE_HPP
#define STRAITX_GRAPHICS_PIPELINE_HPP

#include "graphics/api/shader.hpp"
#include "graphics/api/render_pass.hpp"

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
	GreaterOrEqual,
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
	//const class DescriptorSetLayout *DescriptorSetLayout;
};
//OpenGL said that
constexpr size_t MaxVertexAttributes = 8;

size_t GetVertexAttributeSize(VertexAttribute attribute);

size_t CalculateStride(Span<VertexAttribute> attributes);

class GraphicsPipeline: public NonCopyable{
public:
    virtual ~GraphicsPipeline();

    virtual ConstSpan<VertexAttribute> VertexAttributes()const = 0;

    static GraphicsPipeline *Create(const GraphicsPipelineProperties &props);
};

#endif//STRAITX_GRAPHICS_PIPELINE_HPP