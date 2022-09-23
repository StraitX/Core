#ifndef STRAITX_TEXTURE_HPP
#define STRAITX_TEXTURE_HPP

#include "core/types.hpp"
#include "core/math/vector2.hpp"
#include "graphics/image.hpp"
#include "graphics/api/graphics_resource.hpp"

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

enum class TextureFormat : u8{
    Unknown = 0,
    RGBA8,
    RGB8,
    RGBA16F,
    BGRA8,
    Depth32,
    Depth24Stencil8
};

namespace Vk{class CommandBufferImpl;}//namespace Vk::

bool IsDepthFormat(TextureFormat format);

bool IsStencilFormat(TextureFormat format);

bool IsColorFormat(TextureFormat format);

size_t GetPixelSize(TextureFormat format);

// NOTE: Don't mess them up, these are tied to vulkan spec

using TextureUsage = u32;

namespace TextureUsageBits{

    enum Values: TextureUsage{
        TransferSrc             = 0x00000001,
        TransferDst             = 0x00000002,
        Sampled                 = 0x00000004,
        Storage                 = 0x00000008,
        ColorAttachmentOptimal  = 0x00000010,
        DepthStencilOptimal     = 0x00000020,
    };

}//namespace TextureUsageBits::

class Texture: public GraphicsResource{
protected:
    TextureFormat m_Format = TextureFormat::Unknown;
    TextureUsage m_Usage = 0;
    mutable TextureLayout m_Layout = TextureLayout::Undefined;

    friend class Vk::CommandBufferImpl;
public:
    TextureFormat Format()const{
        return m_Format;
    }

    TextureUsage Usage()const{
        return m_Usage;
    }

    TextureLayout Layout()const{
        return m_Layout;
    }

    virtual void ChangeLayout(TextureLayout new_layout)const = 0;
};

class Texture2D: public Texture{
protected:
    u32 m_Width = 0;
    u32 m_Height = 0;
public:
    Texture2D() = default;
    
    virtual ~Texture2D() = default;

    virtual void Copy(void *src_data, Vector2u src_size) = 0;

    void Copy(const Image &src_image){
        Copy(src_image.Data(), {src_image.Width(), src_image.Height()});
    }

    Vector2u Size()const{
        return {m_Width, m_Height};
    }
public:
    static Texture2D *Create(u32 width, u32 height, TextureFormat format, TextureUsage usage, TextureLayout initial_layout = TextureLayout::Undefined);

    static Texture2D* Create(Vector2u size, TextureFormat format, TextureUsage usage, TextureLayout initial_layout = TextureLayout::Undefined) {
        return Create(size.x, size.y, format, usage, initial_layout);
    }

    static Texture2D *Create(const Image &image, TextureUsage usage = TextureUsageBits::Sampled | TextureUsageBits::TransferDst, TextureLayout initial_layout = TextureLayout::ShaderReadOnlyOptimal);

    static Texture2D *Create(const char *filepath, TextureUsage usage = TextureUsageBits::Sampled | TextureUsageBits::TransferDst, TextureLayout initial_layout = TextureLayout::ShaderReadOnlyOptimal);
};

#endif//STRAITX_TEXTURE_HPP