#ifndef STRAITX_IMAGE_HPP
#define STRAITX_IMAGE_HPP

#include "platform/types.hpp"
#include "platform/file.hpp"
#include "core/math/vector2.hpp"
#include "core/noncopyable.hpp"
#include "graphics/api/format.hpp"
#include "graphics/color.hpp"
#include "graphics/image_loader.hpp"

namespace StraitX{

// image is NonCopyable just for now
class Image: public NonCopyable{
private:
    u8 *m_Data = nullptr;
    u32 m_Width = 0;
    u32 m_Height = 0;
    PixelFormat m_Format = {};
public:   
    Image() = default;

    ~Image();

    void Create(u32 width, u32 height, const Color &color = Color::Black);

    Result LoadFromFile(const char *filename, PixelFormat desired_format = PixelFormat::RGBA8);

    Result LoadFromFile(File &file, PixelFormat desired_format = PixelFormat::RGBA8);

    Result SaveToFile(File &file, ImageFormat save_format);

    Result SaveToFile(const char *filename);

    constexpr u8 *Data()const;

    constexpr u32 Width()const;

    constexpr u32 Height()const;

    constexpr Vector2u Size()const;

    constexpr PixelFormat Format()const;
};

constexpr u8 *Image::Data()const{
    return m_Data;
}

constexpr u32 Image::Width()const{
    return m_Width;
}

constexpr u32 Image::Height()const{
    return m_Height;
}

constexpr Vector2u Image::Size()const{
    return {Width(), Height()};
}

constexpr PixelFormat Image::Format()const{
    return m_Format;
}

};//namespace StraitX::

#endif//STRAITX_IMAGE_HPP