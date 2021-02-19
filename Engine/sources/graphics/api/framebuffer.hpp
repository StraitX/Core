#ifndef STRAITX_FRAMEBUFFER_HPP
#define STRAITX_FRAMEBUFFER_HPP

#include "core/array_ptr.hpp"
#include "core/push_array.hpp"
#include "core/noncopyable.hpp"
#include "core/math/vector2.hpp"
#include "graphics/api/gpu_configuration.hpp"
#include "graphics/api/gpu_texture.hpp"
#include "graphics/api/render_pass.hpp"

namespace StraitX{

struct FramebufferProperties{
    Vector2u Size = {0, 0};
    ArrayPtr<const GPUTexture* const> Attachments = {};
};

class GraphicsAPILoader;

class Framebuffer: NonCopyable{
public:
    struct VTable{
        using NewProc    = Framebuffer *(*)(LogicalGPU &owner, const RenderPass *const pass, const FramebufferProperties &props);
        using DeleteProc = void (*)(Framebuffer *framebuffer);

        NewProc    New    = nullptr;
        DeleteProc Delete = nullptr;
    };
private:
    static VTable s_VTable;

    Vector2u m_Size;
    PushArray<const GPUTexture *, MaxAttachmentsCount> m_Attachments;

    friend class GraphicsAPILoader;
public:
    Framebuffer(const RenderPass *const pass, const FramebufferProperties &props);

    virtual ~Framebuffer() = default;

    constexpr Vector2u Size()const;

    constexpr const PushArray<const GPUTexture *, MaxAttachmentsCount>& Attachments()const;

    static Framebuffer *New(const RenderPass *const pass, const FramebufferProperties &props);

    static void Delete(Framebuffer *framebuffer);

};


constexpr Vector2u Framebuffer::Size()const{ 
    return m_Size; 
}

constexpr const PushArray<const GPUTexture *, MaxAttachmentsCount>& Framebuffer::Attachments()const{
    return m_Attachments;
}

sx_inline Framebuffer *Framebuffer::New(const RenderPass *const pass,const FramebufferProperties &props){
    return s_VTable.New(LogicalGPU::Instance(), pass, props);
}

sx_inline void Framebuffer::Delete(Framebuffer *framebuffer){
    s_VTable.Delete(framebuffer);
}

}//namespace StraitX::

#endif//STRAITX_FRAMEBUFFER_HPP