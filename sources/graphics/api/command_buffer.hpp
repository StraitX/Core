#ifndef STRAITX_COMMAND_BUFFER_HPP
#define STRAITX_COMMAND_BUFFER_HPP

#include "core/noncopyable.hpp"
#include "graphics/api/texture.hpp"

class RenderPass;
class Framebuffer;
class GraphicsPipeline;

class CommandBuffer;

class CommandPool: public NonCopyable{
public:
    CommandPool() = default;

    virtual ~CommandPool() = default;

    virtual CommandBuffer *Alloc() = 0;

    virtual void Free(CommandBuffer *buffer) = 0;

    //virtual void Reset() = 0

    static CommandPool *Create();
};

class CommandBuffer: public NonCopyable{
public:
    CommandBuffer() = default;

    virtual ~CommandBuffer() = default;

    virtual void Begin() = 0;

    virtual void End() = 0;

    virtual void Reset() = 0;

    virtual void ChangeLayout(Texture2D *texture, TextureLayout new_layout) = 0;

    virtual void BeginRenderPass(const RenderPass *rp, const Framebuffer *fb) = 0;

    virtual void EndRenderPass() = 0;

    virtual void Bind(const GraphicsPipeline *pipeline) = 0;

    virtual void Draw(u32 vertices_count) = 0;

    virtual void SetScissor(s32 x, s32 y, u32 width, u32 height) = 0;

    virtual void SetViewport(s32 x, s32 y, u32 width, u32 height) = 0;

};

#endif//STRAITX_COMMAND_BUFFER_HPP