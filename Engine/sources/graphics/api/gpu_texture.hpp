#ifndef STRAITX_GPU_TEXTURE_HPP
#define STRAITX_GPU_TEXTURE_HPP

#include "core/noncopyable.hpp"
#include "core/math/vector2.hpp"
#include "graphics/api/gpu_configuration.hpp"
#include "graphics/api/format.hpp"
#include "graphics/api/cpu_buffer.hpp"
#include "graphics/api/logical_gpu.hpp"

namespace StraitX{

namespace GL{
class GPUTextureImpl;
class GPUContextImpl;
class DMAImpl;
}//namespace GL::

namespace Vk{
class GPUTextureImpl;
class GPUContextImpl;
class DMAImpl;
}//namespace Vk::

enum class SamplePoints{
    Samples_1 = 0,
    Samples_2 = 1,
    Samples_4 = 2,
    Samples_8 = 3,
    Samples_16= 4 
};

class GraphicsAPILoader;

class GPUTexture: public NonCopyable{
public:
    enum class Layout : u8{
        Undefined = 0,
        General,
        ColorAttachmentOptimal,
        DepthStencilAttachmentOptimal,
        TransferSrcOptimal,
        TransferDstOptimal,
        PresentSrcOptimal,
        ShaderReadOnlyOptimal
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
    struct VTable{
        using NewProc    = void (*)(GPUTexture &texture, TextureFormat format, Usage usage, u32 width, u32 height);
        using DeleteProc = void (*)(GPUTexture &texture);

        NewProc    New    = nullptr;
        DeleteProc Delete = nullptr;
    };
private:
    static VTable s_VTable;

    LogicalGPU * m_Owner = nullptr;
    GPUResourceHandle m_Handle = {};
    GPUResourceHandle m_ViewHandle = {};
    GPUResourceHandle m_BackingMemory = {};
    u32 m_Width  = 0;
    u32 m_Height = 0;
    Layout m_Layout = Layout::Undefined;
    TextureFormat m_Format = TextureFormat::Unknown;
    Usage m_Usage = {};

    friend class GraphicsAPILoader;
    friend class Vk::GPUTextureImpl;
    friend class GL::GPUTextureImpl;
    friend class Vk::GPUContextImpl;
    friend class GL::GPUContextImpl;
    friend class Vk::DMAImpl;
    friend class GL::DMAImpl;
public:

    constexpr GPUTexture() = default;

    sx_inline void New(TextureFormat format, Usage usage, u32 width, u32 height);

    sx_inline void Delete();

    constexpr LogicalGPU *Owner()const;

    constexpr GPUResourceHandle Handle()const;

    constexpr GPUResourceHandle ViewHandle()const;

    constexpr Vector2u Size()const;

    constexpr TextureFormat GetFormat()const;

    constexpr Layout GetLayout()const;

    constexpr Usage GetUsage()const;

};

sx_inline void GPUTexture::New(TextureFormat format, Usage usage, u32 width, u32 height){
    m_Owner = &LogicalGPU::Instance();
    s_VTable.New(*this, format, usage, width, height);
}

sx_inline void GPUTexture::Delete(){
    s_VTable.Delete(*this);
}

constexpr LogicalGPU *GPUTexture::Owner()const{
    return m_Owner;
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

constexpr TextureFormat GPUTexture::GetFormat()const{ 
    return m_Format;
}

constexpr GPUTexture::Layout GPUTexture::GetLayout()const{
    return m_Layout;
}

constexpr GPUTexture::Usage GPUTexture::GetUsage()const{
    return m_Usage;
}

}//namespace StraitX::

#endif //STRAITX_GPU_TEXTURE_HPP 