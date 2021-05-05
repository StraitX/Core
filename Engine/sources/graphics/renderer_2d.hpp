#ifndef STRAITX_RENDERER_2D_HPP
#define STRAITX_RENDERER_2D_HPP

#include "core/math/matrix4.hpp"
#include "core/math/vector2.hpp"
#include "core/pair.hpp"
#include "graphics/api/gpu_buffer.hpp"
#include "graphics/api/cpu_buffer.hpp"
#include "graphics/api/gpu_texture.hpp"
#include "graphics/api/shader.hpp"
#include "graphics/api/render_pass.hpp"
#include "graphics/api/graphics_pipeline.hpp"
#include "graphics/api/framebuffer.hpp"
#include "graphics/color.hpp"
#include "graphics/texture.hpp"
#include "graphics/image.hpp"

namespace StraitX{

class Renderer2D: public NonCopyable{
public:
    struct Vertex2D{
        Vector2f a_Position;
        Vector2f a_TexCoords;
        float a_TexIndex;
        Color a_Color;
    };

    struct UniformData{
        Matrix4f u_Projection;
    };
private:
    static constexpr s32 MaxTextures = 8;
    static constexpr size_t MaxVerticesCount = 400000;
    static constexpr size_t MaxIndicesCount  = 600000;

    Texture m_WhiteTexture{Image(1,1,Color::White)};

    Shader *m_VertexShader = nullptr;
    Shader *m_FragmentShader = nullptr;

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

    PushArray<const Texture *, MaxTextures> m_Textures;
    
    GPUBuffer m_VertexBuffer;
    GPUBuffer m_IndexBuffer;
    GPUBuffer m_UniformBuffer;

    Vector2i m_CameraPosition = {};
    Vector2i m_WindowSize;
public:
    Renderer2D(const RenderPass *pass);

    ~Renderer2D();

    void BeginScene(const Framebuffer *framebuffer, Vector2i cam_position);

    void EndScene();

    void DrawRect(Vector2i position, Vector2i size, const Color &color, const Texture &texture);

    void DrawRect(Vector2i position, Vector2i size, const Color &color);

    void DrawRect(Vector2i position, Vector2i size, const Texture &texture);

private:
    void BeginBatch();
    
    void EndBatch();
};

}//namespace StraitX::

#endif//STRAITX_RENDERER_2D_HPP