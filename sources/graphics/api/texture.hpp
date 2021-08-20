#ifndef STRAITX_TEXTURE_HPP
#define STRAITX_TEXTURE_HPP

#include "core/types.hpp"
#include "core/math/vector2.hpp"
#include "core/noncopyable.hpp"

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
    BGRA8,
    Depth32,
    Depth24Stencil8
};

bool IsDepthFormat(TextureFormat format);

// NOTE: Don't mess them up, these are tied to vulkan spec

using TextureUsage = u32;

namespace TextureUsageBits{

    enum Values: TextureUsage{
        TransferSrc             = 0x00000001,
        TransferDst             = 0x00000002,
        Sampled                 = 0x00000004,
        ColorAttachmentOptimal  = 0x00000010,
        DepthStencilOptimal     = 0x00000020,
    };

}//namespace TextureUsageBits::

class Texture: public NonCopyable{
protected:
    TextureFormat m_Format = TextureFormat::Unknown;
    TextureUsage m_Usage = 0;
    TextureLayout m_Layout = TextureLayout::Undefined;
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

    //virtual void ChangeLayout(TextureLayout layout) = 0;
};

class Texture2D: public Texture{
protected:
    u32 Width = 0;
    u32 Height = 0;
public:
    //virtual void UploadData(void *pixels);

    //virtual void UploadImage(const Image &image);

    Vector2u Size()const{
        return {Width, Height};
    }
public:
    static Texture2D *Create(u32 width, u32 height, TextureFormat format, TextureUsage usage, TextureLayout initial_layout = TextureLayout::Undefined);
};

#endif//STRAITX_TEXTURE_HPP