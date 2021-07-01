#ifndef STRAITX_GPU_TEXTURE_HPP
#define STRAITX_GPU_TEXTURE_HPP

#include "platform/compiler.hpp"
#include "core/noncopyable.hpp"
#include "graphics/api/gpu_configuration.hpp"
#include "graphics/api/format.hpp"
#include "graphics/api/cpu_buffer.hpp"
#include "graphics/api/sampler.hpp"

namespace StraitX{

enum class SamplePoints{
    Samples_1 = 0,
    Samples_2 = 1,
    Samples_4 = 2,
    Samples_8 = 3,
    Samples_16= 4 
};

enum class TextureLayout : u8{
	Undefined = 0,
	General,
	ColorAttachmentOptimal,
	DepthStencilAttachmentOptimal,
	TransferSrcOptimal,
	TransferDstOptimal,
	PresentSrcOptimal,
	ShaderReadOnlyOptimal
};

// NOTE: Don't mess them up, these are tied to vulkan spec
enum class TextureUsageBits: u32{
	TransferSrc             = 0x00000001,
	TransferDst             = 0x00000002,
	Sampled                 = 0x00000004,
	ColorAttachmentOptimal  = 0x00000010,
	DepthStencilOptimal     = 0x00000020,
};

class Texture: public NonCopyable{
protected:
    GPUResourceHandle m_Handle = {};
    GPUResourceHandle m_ViewHandle = {};
    GPUResourceHandle m_BackingMemory = {};
    TextureLayout m_Layout = TextureLayout::Undefined;
    TextureFormat m_Format = TextureFormat::Unknown;
    TextureUsageBits m_Usage = {};
public:
    Texture() = default;

    Texture(Texture &&other);

    ~Texture() = default;

    Texture &operator=(Texture &&other);

    GPUResourceHandle Handle()const;

    GPUResourceHandle ViewHandle()const;

    TextureFormat Format()const;

    TextureLayout Layout()const;

    TextureUsageBits Usage()const;

    bool IsEmpty()const;
private:
    void SetZero();
};

class Texture2D: public Texture{
public:
	struct VTable{
        using NewProc    = void (*)(Texture2D &texture, u32 width, u32 height, TextureFormat format, TextureUsageBits usage);
        using DeleteProc = void (*)(Texture2D &texture);

        NewProc    New    = nullptr;
        DeleteProc Delete = nullptr;
    };
private:
    static VTable s_VTable;

	friend class GraphicsAPILoader;
private:
	u32 m_Width;
	u32 m_Height;
	Sampler m_Sampler;
public:
	void New(u32 width, u32 height, TextureFormat format, TextureUsageBits usage, const SamplerProperties &props = {});

	void Delete();

	u32 Width()const;

	u32 Height()const;

	const class Sampler &Sampler()const;
};

/*
class TextureCube: public Texture{
private:
	u32 m_Size;
	Sampler m_Sampler;
public:
	void New(u32 size, TextureFormat format, TextureUsageBits usage, const SamplerProperties &props);

	void Delete();
};
*/

SX_INLINE Texture::Texture(Texture &&other){
    *this = Move(other);
}


SX_INLINE Texture &Texture::operator=(Texture &&other){
    SX_CORE_ASSERT(IsEmpty(), "Texture: Can't move into non-empty object");
    m_Handle = other.m_Handle;
    m_ViewHandle = other.m_ViewHandle;
    m_BackingMemory = other.m_BackingMemory;
    m_Layout = other.m_Layout;
    m_Format = other.m_Format;
    m_Usage = other.m_Usage;
    other.SetZero();
    return *this;
}

SX_INLINE GPUResourceHandle Texture::Handle()const{ 
    return m_Handle; 
}

SX_INLINE GPUResourceHandle Texture::ViewHandle()const{ 
    return m_ViewHandle; 
}

SX_INLINE TextureFormat Texture::Format()const{ 
    return m_Format;
}

SX_INLINE TextureLayout Texture::Layout()const{
    return m_Layout;
}

SX_INLINE TextureUsageBits Texture::Usage()const{
    return m_Usage;
}

SX_INLINE bool Texture::IsEmpty()const{
    return m_Handle.U64 == 0;
}

SX_INLINE void Texture::SetZero(){
    m_Handle = {};
    m_ViewHandle = {};
    m_BackingMemory = {};
    m_Layout = TextureLayout::Undefined;
    m_Format = TextureFormat::Unknown;
    m_Usage = {};
}

SX_INLINE void Texture2D::New(u32 width, u32 height, TextureFormat format, TextureUsageBits usage, const SamplerProperties &props){
	m_Sampler.New(props);

	s_VTable.New(*this, width, height, format, usage);
}

SX_INLINE void Texture2D::Delete(){
	s_VTable.Delete(*this);

	m_Sampler.Delete();	
}

SX_INLINE u32 Texture2D::Width()const{
	return m_Width;
}

SX_INLINE u32 Texture2D::Height()const{
	return m_Height;
}

SX_INLINE const class Sampler &Texture2D::Sampler()const{
	return m_Sampler;
}

}//namespace StraitX::

#endif //STRAITX_GPU_TEXTURE_HPP 