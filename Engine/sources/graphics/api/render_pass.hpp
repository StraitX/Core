#ifndef STRAITX_RENDER_PASS_HPP
#define STRAITX_RENDER_PASS_HPP

#include "core/span.hpp"
#include "core/push_array.hpp"
#include "core/math/vector4.hpp"
#include "core/noncopyable.hpp"
#include "graphics/api/texture.hpp"

namespace StraitX{

struct AttachmentDescription{
    TextureLayout InitialLayout = TextureLayout::Undefined;
    TextureLayout FinalLayout = TextureLayout::Undefined;
    TextureLayout InPassLayout = TextureLayout::Undefined;
    TextureFormat Format = TextureFormat::Unknown;
    SamplePoints Samples = SamplePoints::Samples_1;
};

struct RenderPassProperties{
    Span<AttachmentDescription> Attachments;
};

class GraphicsAPILoader;

// Due to OpenGL Limitations
constexpr size_t MaxAttachmentsCount = 8;

class RenderPass: NonCopyable{
public:
    struct VTable{
        using NewProc    = RenderPass *(*)(const RenderPassProperties &props);
        using DeleteProc = void (*)(RenderPass *pass);

        NewProc    New    = nullptr;
        DeleteProc Delete = nullptr;
    };
private:
    static VTable s_VTable;

    friend class GraphicsAPILoader;
public:

    virtual ~RenderPass() = default;

    static RenderPass *New(const RenderPassProperties &properties);

    static void Delete(RenderPass *pass);
};

SX_INLINE RenderPass *RenderPass::New(const RenderPassProperties &properties){
    return s_VTable.New(properties);
}

SX_INLINE void RenderPass::Delete(RenderPass *pass){
    s_VTable.Delete(pass);
}

}//namespace StraitX::

#endif//STRAITX_RENDER_PASS_HPP