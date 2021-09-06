#ifndef STRAITX_FRAMEBUFFER_HPP
#define STRAITX_FRAMEBUFFER_HPP

#include "core/math/vector2.hpp"
#include "core/span.hpp"
#include "core/noncopyable.hpp"
#include "graphics/api/texture.hpp"
#include "graphics/api/render_pass.hpp"

struct FramebufferProperties{
    Vector2u Size = {0, 0};
    ConstSpan<Texture2D *> Attachments = {};
    const RenderPass *Pass = nullptr;
};

class Framebuffer: public NonCopyable{
public:
    virtual ~Framebuffer() = default;

    virtual Vector2u Size()const = 0;

    virtual ConstSpan<Texture2D *> Attachments()const = 0;

    static Framebuffer *Create(const FramebufferProperties &props);
};

#endif//STRAITX_FRAMEBUFFER_HPP