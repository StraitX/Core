#ifndef STRAITX_RENDER_PASS_HPP
#define STRAITX_RENDER_PASS_HPP

#include "core/array_ptr.hpp"
#include "core/push_array.hpp"
#include "core/math/vector4.hpp"
#include "core/noncopyable.hpp"
#include "graphics/api/logical_gpu.hpp"
#include "graphics/api/gpu_texture.hpp"

namespace StraitX{

struct AttachmentDescription{
    GPUTexture::Layout InitialLayout = GPUTexture::Layout::Undefined;
    GPUTexture::Layout FinalLayout = GPUTexture::Layout::Undefined;
    GPUTexture::Layout InPassLayout = GPUTexture::Layout::Undefined;
    GPUTexture::Format Format = GPUTexture::Format::Unknown;
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
        using NewProc    = RenderPass *(*)(const LogicalGPU &owner, const RenderPassProperties &props);
        using DeleteProc = void (*)(RenderPass *pass);

        NewProc    New    = nullptr;
        DeleteProc Delete = nullptr;
    };
private:
    static VTable s_VTable;
    PushArray<AttachmentDescription, MaxAttachmentsCount> m_Attachments;
    bool m_HasDepth = false;

    friend class GraphicsAPILoader;
public:

    RenderPass(const ArrayPtr<const AttachmentDescription> &attachments);

    virtual ~RenderPass() = default;

    constexpr const PushArray<AttachmentDescription, MaxAttachmentsCount> &Attachments()const;

    constexpr bool HasDepth()const;

    sx_inline static RenderPass *New(const RenderPassProperties &properties);

    sx_inline static void Delete(RenderPass *pass);
};

constexpr const PushArray<AttachmentDescription, MaxAttachmentsCount> &RenderPass::Attachments()const{
    return m_Attachments;
}

constexpr bool RenderPass::HasDepth()const{
    return m_HasDepth;
}

sx_inline RenderPass *RenderPass::New(const RenderPassProperties &properties){
    return s_VTable.New(LogicalGPU::Instance(), properties);
}

sx_inline void RenderPass::Delete(RenderPass *pass){
    s_VTable.Delete(pass);
}

}//namespace StraitX::

#endif//STRAITX_RENDER_PASS_HPP