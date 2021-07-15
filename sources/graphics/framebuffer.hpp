#ifndef STRAITX_FRAMEBUFFER_HPP
#define STRAITX_FRAMEBUFFER_HPP

#include "core/span.hpp"
#include "core/push_array.hpp"
#include "core/noncopyable.hpp"
#include "core/math/vector2.hpp"
#include "graphics/gpu_configuration.hpp"
#include "graphics/texture.hpp"
#include "graphics/render_pass.hpp"

struct FramebufferProperties{
    Vector2u Size = {0, 0};
    Span<const Texture2D* const> Attachments = {};
};

class GraphicsAPILoader;

class Framebuffer: public NonCopyable{
public:
    struct VTable{
        using NewProc    = Framebuffer *(*)(const RenderPass *const pass, const FramebufferProperties &props);
        using DeleteProc = void (*)(Framebuffer *framebuffer);

        NewProc    New    = nullptr;
        DeleteProc Delete = nullptr;
    };
protected:
    static VTable s_VTable;

    const RenderPass *m_Pass;

    friend class GraphicsAPILoader;
public:
    Framebuffer(const RenderPass *pass);

    virtual ~Framebuffer() = default;

    virtual Vector2u Size()const = 0;

    const RenderPass *Pass()const;

    static Framebuffer *New(const RenderPass *const pass, const FramebufferProperties &props);

    static void Delete(Framebuffer *framebuffer);

};

SX_INLINE Framebuffer::Framebuffer(const RenderPass *pass):
    m_Pass(pass)
{}

SX_INLINE const RenderPass *Framebuffer::Pass()const{
    return m_Pass;
}

SX_INLINE Framebuffer *Framebuffer::New(const RenderPass *const pass,const FramebufferProperties &props){
    return s_VTable.New(pass, props);
}

SX_INLINE void Framebuffer::Delete(Framebuffer *framebuffer){
    s_VTable.Delete(framebuffer);
}

#endif//STRAITX_FRAMEBUFFER_HPP