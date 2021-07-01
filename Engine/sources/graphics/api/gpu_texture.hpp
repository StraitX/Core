#ifndef STRAITX_GPU_TEXTURE_HPP
#define STRAITX_GPU_TEXTURE_HPP

#include "platform/compiler.hpp"
#include "core/noncopyable.hpp"
#include "core/math/vector2.hpp"
#include "graphics/api/gpu_configuration.hpp"
#include "graphics/api/format.hpp"
#include "graphics/api/cpu_buffer.hpp"

namespace StraitX{

namespace GL{
class GPUTextureImpl;
class GraphicsContextImpl;
class DMAImpl;
}//namespace GL::

namespace Vk{
class GPUTextureImpl;
class GraphicsContextImpl;
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
	friend class Vk::GraphicsContextImpl;
	friend class GL::GraphicsContextImpl;
    friend class Vk::DMAImpl;
    friend class GL::DMAImpl;
public:

    GPUTexture() = default;

    GPUTexture(GPUTexture &&other);

    ~GPUTexture();

    GPUTexture &operator=(GPUTexture &&other);

    void New(TextureFormat format, Usage usage, u32 width, u32 height);

    void Delete();

    GPUResourceHandle Handle()const;

    GPUResourceHandle ViewHandle()const;

    Vector2u Size()const;

    TextureFormat GetFormat()const;

    Layout GetLayout()const;

    Usage GetUsage()const;

    bool IsEmpty()const;
private:
    void SetZero();
};

SX_INLINE GPUTexture::GPUTexture(GPUTexture &&other){
    *this = Move(other);
}

SX_INLINE GPUTexture::~GPUTexture(){
    if(!IsEmpty())
        Delete();
}

SX_INLINE GPUTexture &GPUTexture::operator=(GPUTexture &&other){
    SX_CORE_ASSERT(IsEmpty(), "GPUTexture: Can't move into non-empty object");
    m_Handle = other.m_Handle;
    m_ViewHandle = other.m_ViewHandle;
    m_BackingMemory = other.m_BackingMemory;
    m_Width  = other.m_Width ;
    m_Height = other.m_Height;
    m_Layout = other.m_Layout;
    m_Format = other.m_Format;
    m_Usage = other.m_Usage;
    other.SetZero();
    return *this;
}

SX_INLINE void GPUTexture::New(TextureFormat format, Usage usage, u32 width, u32 height){
    s_VTable.New(*this, format, usage, width, height);
}

SX_INLINE void GPUTexture::Delete(){
    s_VTable.Delete(*this);
    SetZero();
}

SX_INLINE GPUResourceHandle GPUTexture::Handle()const{ 
    return m_Handle; 
}

SX_INLINE GPUResourceHandle GPUTexture::ViewHandle()const{ 
    return m_ViewHandle; 
}

SX_INLINE Vector2u GPUTexture::Size()const{ 
    return {m_Width, m_Height}; 
}

SX_INLINE TextureFormat GPUTexture::GetFormat()const{ 
    return m_Format;
}

SX_INLINE GPUTexture::Layout GPUTexture::GetLayout()const{
    return m_Layout;
}

SX_INLINE GPUTexture::Usage GPUTexture::GetUsage()const{
    return m_Usage;
}

SX_INLINE bool GPUTexture::IsEmpty()const{
    return m_Handle.U64 == 0;
}

SX_INLINE void GPUTexture::SetZero(){
    m_Handle = {};
    m_ViewHandle = {};
    m_BackingMemory = {};
    m_Width  = 0;
    m_Height = 0;
    m_Layout = Layout::Undefined;
    m_Format = TextureFormat::Unknown;
    m_Usage = {};
}

}//namespace StraitX::

#endif //STRAITX_GPU_TEXTURE_HPP 