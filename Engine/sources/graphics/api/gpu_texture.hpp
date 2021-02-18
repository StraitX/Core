#ifndef STRAITX_GPU_TEXTURE_HPP
#define STRAITX_GPU_TEXTURE_HPP

#include "core/noncopyable.hpp"
#include "core/math/vector2.hpp"
#include "graphics/api/gpu_configuration.hpp"
#include "graphics/api/cpu_buffer.hpp"
#include "graphics/api/logical_gpu.hpp"

namespace StraitX{

namespace GL{
class GPUTextureImpl;
}//namespace GL::

namespace Vk{
class GPUTextureImpl;
}//namespace Vk::

enum class SamplePoints{
    Samples_1 = 0,
    Samples_2 = 1,
    Samples_4 = 2,
    Samples_8 = 3,
    Samples_16= 4 
};

class GraphicsAPILoader;

class GPUTexture: NonCopyable{
public:
    enum class Layout : u8{
        Undefined = 0,
        General,
        ColorAttachmentOptimal,
        DepthStencilAttachmentOptimal,
        TransferSrcOptimal,
        TransferDstOptimal,
        PresentSrcOptimal
    };

    using Usage = u32;
    // NOTE: Don't mess them up, these are tied to vulkan spec
    enum UsageBits: Usage{
        TransferSrc             = 0x00000001,
        TransferDst             = 0x00000002,
        Sampled                 = 0x00000004,
        ColorAttachmentOptimal  = 0x00000010,
        DepthStencilOptimal     = 0x00000020,
    };
    enum class Format : u8{
        Unknown = 0,
        RGBA8,
        Depth24Stencil8
    };
    struct VTable{
        using NewProc    = void (*)(GPUTexture &texture, Format format, Usage usage, u32 width, u32 height);
        using DeleteProc = void (*)(GPUTexture &texture);

        NewProc    New    = nullptr;
        DeleteProc Delete = nullptr;
    };
private:
    static VTable s_VTable;

    LogicalGPU *const m_Owner = nullptr;
    GPUResourceHandle m_Handle = {};
    GPUResourceHandle m_ViewHandle = {};
    GPUResourceHandle m_BackingMemory = {};
    u32 m_Width  = 0;
    u32 m_Height = 0;
    Layout m_Layout;
    Format m_Format;
    Usage m_Usage = {};

    friend class GraphicsAPILoader;
    friend class Vk::GPUTextureImpl;
    friend class GL::GPUTextureImpl;
public:

    sx_inline GPUTexture():
        m_Owner(&LogicalGPU::Instance())
    {}

    sx_inline void New(Format format, Usage usage, u32 width, u32 height){
        s_VTable.New(*this, format, usage, width, height);
    }

    sx_inline void Delete(){
        s_VTable.Delete(*this);
    }
    sx_inline GPUResourceHandle Handle()const{ return m_Handle; }

    sx_inline GPUResourceHandle ViewHandle()const{ return m_ViewHandle; }

    sx_inline Vector2u Size()const{ return {m_Width, m_Height}; }

    sx_inline Format GetFormat()const{ return m_Format; }

    static bool IsDepthFormat(GPUTexture::Format format);
};

}//namespace StraitX::

#endif //STRAITX_GPU_TEXTURE_HPP 