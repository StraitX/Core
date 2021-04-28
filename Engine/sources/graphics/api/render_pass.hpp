#ifndef STRAITX_RENDER_PASS_HPP
#define STRAITX_RENDER_PASS_HPP

#include "core/array_ptr.hpp"
#include "core/push_array.hpp"
#include "core/math/vector4.hpp"
#include "core/noncopyable.hpp"
#include "graphics/api/gpu_texture.hpp"

namespace StraitX{

struct AttachmentDescription{
    GPUTexture::Layout InitialLayout = GPUTexture::Layout::Undefined;
    GPUTexture::Layout FinalLayout = GPUTexture::Layout::Undefined;
    GPUTexture::Layout InPassLayout = GPUTexture::Layout::Undefined;
    TextureFormat Format = TextureFormat::Unknown;
    SamplePoints Samples = SamplePoints::Samples_1;
};

struct RenderPassProperties{
    ArrayPtr<const AttachmentDescription> Attachments;
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

    sx_inline static RenderPass *New(const RenderPassProperties &properties);

    sx_inline static void Delete(RenderPass *pass);
};

sx_inline RenderPass *RenderPass::New(const RenderPassProperties &properties){
    return s_VTable.New(properties);
}

sx_inline void RenderPass::Delete(RenderPass *pass){
    s_VTable.Delete(pass);
}

}//namespace StraitX::

#endif//STRAITX_RENDER_PASS_HPP