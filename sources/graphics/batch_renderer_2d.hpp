#ifndef STRAITX_BATCH_RENDERER_2D_HPP
#define STRAITX_BATCH_RENDERER_2D_HPP

#include "core/math/matrix4.hpp"
#include "core/math/vector2.hpp"
#include "core/pair.hpp"
#include "graphics/gpu_buffer.hpp"
#include "graphics/cpu_buffer.hpp"
#include "graphics/texture.hpp"
#include "graphics/shader.hpp"
#include "graphics/render_pass.hpp"
#include "graphics/graphics_pipeline.hpp"
#include "graphics/framebuffer.hpp"
#include "graphics/descriptor_set.hpp"
#include "graphics/gpu_command_buffer.hpp"
#include "graphics/color.hpp"
#include "graphics/image.hpp"

class BatchRenderer2D: public NonCopyable{
private:
    struct Vertex2D{
        Vector2f a_Position;
        Vector2f a_TexCoords;
        float a_TexIndex;
        Color a_Color;
    };

    struct UniformData{
        Matrix4f u_Projection;
    };
public:
    static const Vector2f s_DefaultTextureCoords[4];
private:
    static constexpr i32 MaxTextures = DescriptorSetLayout::s_MaxTexturesBindings;
    static constexpr size_t MaxVerticesCount = 400000;
    static constexpr size_t MaxIndicesCount  = 600000;

    Texture2D m_WhiteTexture{Image(1,1,Color::White)};

	GPUCommandBuffer m_CmdBuffer;

    Shader *m_VertexShader = nullptr;
    Shader *m_FragmentShader = nullptr;

	static ShaderBinding s_Bindings[];

	DescriptorSetLayout m_SetLayout;
	DescriptorSetPool *m_SetPool = nullptr;
	DescriptorSet *m_Set = nullptr;

    GraphicsPipeline *m_Pipeline = nullptr;

    const RenderPass *m_Pass = nullptr;
    const Framebuffer *m_CurrentFramebuffer = nullptr;

    CPUBuffer m_StagingIndex;
    CPUBuffer m_StagingVertex;
    CPUBuffer m_StagingUniform;

    Vertex2D *m_Vertices;
    u32 *m_Indices;
    UniformData *m_Uniform;

    u32 m_VerticesCount = 0;
    u32 m_IndicesCount = 0;
    u32 m_DrawCallsCount = 0;
    u32 m_QuadsCount = 0;

    PushArray<const Texture2D *, MaxTextures> m_Textures;
    
    GPUBuffer m_VertexBuffer;
    GPUBuffer m_IndexBuffer;
    GPUBuffer m_UniformBuffer;

    Vector2i m_CameraPosition;
    Vector2u m_WindowSize;
public:
    BatchRenderer2D(const RenderPass *pass);

    ~BatchRenderer2D();

    void BeginScene(const Framebuffer *framebuffer, Vector2i camera_position = {});

    void EndScene();

    void DrawRect(Vector2i position, Vector2i size, const Color &color, const Texture2D &texture, const Vector2f (&texture_coords)[4] = s_DefaultTextureCoords);

    void DrawRect(Vector2i position, Vector2i size, const Color &color);

    void DrawRect(Vector2i position, Vector2i size, const Texture2D &texture, const Vector2f (&texture_coords)[4] = s_DefaultTextureCoords);

	void Clear(const Color &color);

private:
    void BeginBatch();
    
    void EndBatch();
};

#endif//STRAITX_BATCH_RENDERER_2D_HPP