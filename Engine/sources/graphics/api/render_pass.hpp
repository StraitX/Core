#ifndef STRAITX_RENDER_PASS_HPP
#define STRAITX_RENDER_PASS_HPP

#include "core/array_ptr.hpp"
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

    friend class GraphicsAPILoader;
public:

    RenderPass() = default;

    virtual ~RenderPass() = default;

    static sx_inline RenderPass *New(const RenderPassProperties &properties){
        return s_VTable.New(LogicalGPU::Instance(), properties);
    }

    static sx_inline void Delete(RenderPass *pass){
        s_VTable.Delete(pass);
    }
};

}//namespace StraitX::

#endif//STRAITX_RENDER_PASS_HPP