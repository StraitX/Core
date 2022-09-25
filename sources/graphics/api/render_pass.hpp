#ifndef STRAITX_RENDER_PASS_HPP
#define STRAITX_RENDER_PASS_HPP

#include "core/span.hpp"
#include "graphics/api/graphics_resource.hpp"
#include "graphics/api/texture.hpp"

struct AttachmentDescription{
    TextureLayout InitialLayout = TextureLayout::Undefined;
    TextureLayout InPassLayout = TextureLayout::Undefined;
    TextureLayout FinalLayout = TextureLayout::Undefined;
    TextureFormat Format = TextureFormat::Unknown;
    SamplePoints Samples = SamplePoints::Samples_1;
};

struct RenderPassProperties{
    ConstSpan<AttachmentDescription> Attachments;
};

// Due to OpenGL Limitations
constexpr size_t MaxAttachmentsCount = 8;

class RenderPass: public GraphicsResource{
public:
    virtual ~RenderPass() = default;

    virtual ConstSpan<AttachmentDescription> Attachments()const = 0;

    static RenderPass *Create(const RenderPassProperties &props);
};

#endif//STRAITX_RENDER_PASS_HPP