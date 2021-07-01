#ifndef STRAITX_FRAMEBUFFER_HPP
#define STRAITX_FRAMEBUFFER_HPP

#include "core/span.hpp"
#include "core/push_array.hpp"
#include "core/noncopyable.hpp"
#include "core/math/vector2.hpp"
#include "graphics/api/gpu_configuration.hpp"
#include "graphics/api/texture.hpp"
#include "graphics/api/render_pass.hpp"

namespace StraitX{

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
private:
    static VTable s_VTable;

    const RenderPass *m_Pass;
    Vector2u m_Size;

    friend class GraphicsAPILoader;
public:
    Framebuffer(const RenderPass *pass, Vector2u size);

    virtual ~Framebuffer() = default;

    Vector2u Size()const;

    const RenderPass *Pass()const;

    static Framebuffer *New(const RenderPass *const pass, const FramebufferProperties &props);

    static void Delete(Framebuffer *framebuffer);

};

SX_INLINE Framebuffer::Framebuffer(const RenderPass *pass, Vector2u size):
    m_Pass(pass),
    m_Size(size)
{}

SX_INLINE Vector2u Framebuffer::Size()const{
    return m_Size;
}

SX_INLINE const RenderPass *Framebuffer::Pass()const{
    return m_Pass;
}

SX_INLINE Framebuffer *Framebuffer::New(const RenderPass *const pass,const FramebufferProperties &props){
    return s_VTable.New(pass, props);
}

SX_INLINE void Framebuffer::Delete(Framebuffer *framebuffer){
    s_VTable.Delete(framebuffer);
}

}//namespace StraitX::

#endif//STRAITX_FRAMEBUFFER_HPP