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

    LogicalGPU * const m_Owner = nullptr;
    GPUResourceHandle m_Handle = {};
    GPUResourceHandle m_ViewHandle = {};
    GPUResourceHandle m_BackingMemory = {};
    u32 m_Width  = 0;
    u32 m_Height = 0;
    Layout m_Layout = Layout::Undefined;
    Format m_Format = Format::Unknown;
    Usage m_Usage = {};

    friend class GraphicsAPILoader;
    friend class Vk::GPUTextureImpl;
    friend class GL::GPUTextureImpl;
public:

    GPUTexture();

    GPUTexture(GPUTexture &&other);

    GPUTexture &operator=(GPUTexture &&other);

    void New(Format format, Usage usage, u32 width, u32 height);

    void Delete();

    constexpr GPUResourceHandle Handle()const;

    constexpr GPUResourceHandle ViewHandle()const;

    constexpr Vector2u Size()const;

    constexpr Format GetFormat()const;

    static bool IsDepthFormat(GPUTexture::Format format);
};

sx_inline GPUTexture::GPUTexture():
    m_Owner(&LogicalGPU::Instance())
{}

sx_inline void GPUTexture::New(Format format, Usage usage, u32 width, u32 height){
    s_VTable.New(*this, format, usage, width, height);
}

sx_inline void GPUTexture::Delete(){
    s_VTable.Delete(*this);
}

constexpr GPUResourceHandle GPUTexture::Handle()const{ 
    return m_Handle; 
}

constexpr GPUResourceHandle GPUTexture::ViewHandle()const{ 
    return m_ViewHandle; 
}

constexpr Vector2u GPUTexture::Size()const{ 
    return {m_Width, m_Height}; 
}

constexpr GPUTexture::Format GPUTexture::GetFormat()const{ 
    return m_Format;
}

}//namespace StraitX::

#endif //STRAITX_GPU_TEXTURE_HPP 