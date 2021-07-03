#include "platform/memory.hpp"
#include "platform/window_system.hpp"
#include "core/log.hpp"
#include "core/string.hpp"
#include "graphics/batch_renderer_2d.hpp"
#include "graphics/api/dma.hpp"
#include "graphics/api/graphics_context.hpp"

static const char *s_VertexShaderSources = R"(#version 440 core

layout(location = 0)in vec2 a_Position;
layout(location = 1)in vec2 a_TexCoord;
layout(location = 2)in float  a_TexIndex;
layout(location = 3)in vec4 a_Color;

layout(location = 0)out vec4 v_Color;
layout(location = 1)out vec2 v_TexCoord;
layout(location = 2)out float v_TexIndex;

layout(std140, binding = 0)uniform Matrices{
    mat4 u_Projection;
};

void main(void){
    gl_Position = u_Projection * vec4(a_Position.xy, 0, 1.0);
    v_Color = a_Color;
    v_TexCoord = a_TexCoord;
    v_TexIndex = a_TexIndex;
}
)";

static const char *s_FragmentShaderSources = R"(#version 440 core

layout(location = 0)in vec4 v_Color;
layout(location = 1)in vec2 v_TexCoord;
layout(location = 2)in float v_TexIndex;

layout(location = 0)out vec4 f_Color;

layout(binding = 1)uniform sampler2D u_Textures[15];

void main(){
    f_Color = v_Color * texture(u_Textures[int(v_TexIndex)], v_TexCoord);
})";

const Vector2f BatchRenderer2D::s_DefaultTextureCoords[4] = {
    {0.f, 1.f},
    {0.f, 0.f},
    {1.f, 0.f},
    {1.f, 1.f}
};

ShaderBinding BatchRenderer2D::s_Bindings[] = {
	{0, 1,           ShaderBindingType::UniformBuffer, Shader::Vertex},
	{1, MaxTextures, ShaderBindingType::Texture,       Shader::Fragment}
};

BatchRenderer2D::BatchRenderer2D(const RenderPass *pass):
	m_SetLayout(s_Bindings),
    m_Pass(pass)
{
	//XXX set projection matrix based on framebuffer size
    auto size = WindowSystem::Window().Size();
    m_WindowSize = {size.width, size.height};
    
    m_VertexShader = Shader::New(Shader::Type::Vertex, Shader::Lang::GLSL, (const u8*)s_VertexShaderSources, String::Length(s_VertexShaderSources));
    m_FragmentShader = Shader::New(Shader::Type::Fragment, Shader::Lang::GLSL, (const u8*)s_FragmentShaderSources, String::Length(s_FragmentShaderSources));

    SX_CORE_ASSERT(m_VertexShader->IsValid() && m_FragmentShader->IsValid(), "Shaders are not valid");

    const Shader *shaders[2]={
        m_VertexShader,
        m_FragmentShader
    };
    VertexAttribute attributes[] = {
        VertexAttribute::Float32x2,
        VertexAttribute::Float32x2,
        VertexAttribute::Float32x1,
        VertexAttribute::Float32x4
    };

    GraphicsPipelineProperties gp_props = {};
    gp_props.Shaders          	= shaders;
    gp_props.VertexAttributes 	= attributes; 
    gp_props.PrimitivesTopology = PrimitivesTopology::Triangles;
    gp_props.RasterizationMode 	= RasterizationMode::Fill;
    gp_props.BlendFunction      = BlendFunction::Add;
	gp_props.DepthFunction 		= DepthFunction::Always;
    gp_props.SrcBlendFactor 	= BlendFactor::SrcAlpha;
    gp_props.DstBlendFactor 	= BlendFactor::OneMinusSrcAlpha;
    gp_props.Pass           	= m_Pass;
	gp_props.DescriptorSetLayout= &m_SetLayout;

    m_Pipeline = GraphicsPipeline::New(gp_props);
    SX_CORE_ASSERT(m_Pipeline->IsValid(), "GraphicsPipeline is not valid");
    
    m_StagingVertex.New(sizeof(Vertex2D) * MaxVerticesCount);
    m_Vertices = (Vertex2D*)m_StagingVertex.Pointer();

    m_StagingIndex.New(sizeof(u32) * MaxIndicesCount);
    m_Indices = (u32*)m_StagingIndex.Pointer();

    m_StagingUniform.New(sizeof(UniformData));
    m_Uniform = (UniformData*)m_StagingUniform.Pointer();

    m_VertexBuffer.New(m_StagingVertex.Size(),GPUMemoryType::DynamicVRAM, GPUBuffer::VertexBuffer | GPUBuffer::TransferDestination);
    m_IndexBuffer.New(m_StagingIndex.Size(), GPUMemoryType::DynamicVRAM, GPUBuffer::IndexBuffer | GPUBuffer::TransferDestination);
    m_UniformBuffer.New(m_StagingUniform.Size(), GPUMemoryType::DynamicVRAM, GPUBuffer::UniformBuffer | GPUBuffer::TransferDestination);

    m_Uniform->u_Projection = {
        {2.f/size.width, 0.f,             0.f, 0.f},
        {0.f,            2.f/size.height, 0.f, 0.f},
        {0.f,            0.f,             1.f, 0.f},
        {0.f,            0.f,             0.f, 1.f}
    };

    DMA::Copy(m_StagingUniform, m_UniformBuffer);

	m_SetPool = DescriptorSetPool::New(&m_SetLayout, 1);

	m_Set = m_SetPool->AllocateSet();

    //m_Pipeline->Bind(0, 0, m_UniformBuffer);
    m_Set->UpdateUniformBinding(0, 0, m_UniformBuffer);

}

BatchRenderer2D::~BatchRenderer2D(){
    Shader::Delete(m_VertexShader);
    Shader::Delete(m_FragmentShader);

    GraphicsPipeline::Delete(m_Pipeline);

	m_SetPool->FreeSet(m_Set);
	DescriptorSetPool::Delete(m_SetPool);

	m_WhiteTexture.Delete();
}

void BatchRenderer2D::BeginScene(const Framebuffer *framebuffer, Vector2i camera_position){
    m_CurrentFramebuffer = framebuffer;
    m_CameraPosition = camera_position;

    m_DrawCallsCount = 0;
    m_QuadsCount = 0;

    BeginBatch();

	m_WindowSize = framebuffer->Size();

	m_CmdBuffer.SetViewport(m_WindowSize.x, m_WindowSize.y, 0, 0);
	m_CmdBuffer.SetScissors(m_WindowSize.x, m_WindowSize.y, 0, 0);
}

void BatchRenderer2D::EndScene(){
    EndBatch();
    //LogInfo("DrawCalls: %, QuadsCount: %",DrawCallsCount, QuadsCount);
}

void BatchRenderer2D::DrawRect(Vector2i position, Vector2i size, const Color &color, const Texture2D &texture, const Vector2f (&texture_coords)[4]){
    m_QuadsCount++;
    
    if(m_VerticesCount + 4 > MaxVerticesCount || m_IndicesCount + 6 > MaxIndicesCount || m_Textures.Size() == m_Textures.Capacity()){
        EndBatch();
        BeginBatch();
    }
    auto tex = m_Textures.Find(&texture);
    auto index = tex - m_Textures.begin();
    if(tex == m_Textures.end()){
        m_Textures.Emplace(&texture);
		m_Set->UpdateTextureBinding(1, index, (**tex));
    }

    position.x -= m_WindowSize.x/2;
    position.y -= m_WindowSize.y/2;

    position.x -= m_CameraPosition.x;
    position.y -= m_CameraPosition.y;

    m_Vertices[m_VerticesCount + 0] = {Vector2f(position.x,          position.y          ),texture_coords[0],float(index),color};
    m_Vertices[m_VerticesCount + 1] = {Vector2f(position.x,          position.y + size.y ),texture_coords[1],float(index),color};
    m_Vertices[m_VerticesCount + 2] = {Vector2f(position.x + size.x, position.y + size.y ),texture_coords[2],float(index),color};
    m_Vertices[m_VerticesCount + 3] = {Vector2f(position.x + size.x, position.y          ),texture_coords[3],float(index),color};

    m_Indices[m_IndicesCount + 0] = m_VerticesCount + 0;
    m_Indices[m_IndicesCount + 1] = m_VerticesCount + 1;
    m_Indices[m_IndicesCount + 2] = m_VerticesCount + 2;
    m_Indices[m_IndicesCount + 3] = m_VerticesCount + 0;
    m_Indices[m_IndicesCount + 4] = m_VerticesCount + 2;
    m_Indices[m_IndicesCount + 5] = m_VerticesCount + 3;

    m_VerticesCount+= 4;
    m_IndicesCount += 6;
}

void BatchRenderer2D::DrawRect(Vector2i position, Vector2i size, const Color &color){
    DrawRect(position,size, color, m_WhiteTexture);
}

void BatchRenderer2D::DrawRect(Vector2i position, Vector2i size, const Texture2D &texture, const Vector2f (&texture_coords)[4]){
    DrawRect(position,size, Color::White, texture, texture_coords);
}

void BatchRenderer2D::Clear(const Color &color){
	EndBatch();
	BeginBatch();
	m_CmdBuffer.ClearFramebufferColorAttachments(m_CurrentFramebuffer, color);
}

void BatchRenderer2D::BeginBatch(){
    m_VerticesCount = 0;
    m_IndicesCount = 0;

	m_Textures.Clear();
}

void BatchRenderer2D::EndBatch(){
    if(!m_VerticesCount || !m_IndicesCount)return;

	m_CmdBuffer.BindPipeline(m_Pipeline);
	m_CmdBuffer.BindDescriptorSet(m_Set);

	m_CmdBuffer.CopyCPUToGPUBuffer(m_StagingVertex, m_VertexBuffer, sizeof(Vertex2D) * m_VerticesCount);
	m_CmdBuffer.CopyCPUToGPUBuffer(m_StagingIndex, m_IndexBuffer, sizeof(u32) * m_IndicesCount);

	m_CmdBuffer.BeginRenderPass(m_Pass, m_CurrentFramebuffer);
	{
		m_CmdBuffer.BindVertexBuffer(m_VertexBuffer);
		m_CmdBuffer.BindIndexBuffer(m_IndexBuffer, IndicesType::Uint32);
		m_CmdBuffer.DrawIndexed(m_IndicesCount);
	}
	m_CmdBuffer.EndRenderPass();

	GraphicsContext::Get().ExecuteCmdBuffer(m_CmdBuffer);
	m_CmdBuffer.Reset();

    m_DrawCallsCount++;
}