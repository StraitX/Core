#ifndef STRAITX_IMAGE_HPP
#define STRAITX_IMAGE_HPP

#include "core/types.hpp"
#include "core/os/file.hpp"
#include "core/math/vector2.hpp"
#include "core/noncopyable.hpp"
#include "graphics/color.hpp"
#include "graphics/image_loader.hpp"

// image is NonCopyable just for now
class Image: public NonCopyable{
private:
    u32 *m_Data = nullptr;
    u32 m_Width = 0;
    u32 m_Height = 0;
public:   
    Image() = default;

    Image(u32 width, u32 height, const Color &color = Color::Black);

    Image(Image &&other)noexcept;

    ~Image();

    Image &operator=(Image &&other)noexcept;

    void Create(u32 width, u32 height, const Color &color = Color::Black);

    void Free();

    Result LoadFromFile(const char *filename);

    Result LoadFromFile(File &file);

    Result SaveToFile(File &file, ImageFileFormat save_format);

    Result SaveToFile(const char *filename);

    void Fill(const Color &color);

    void *Data()const;

    u32 Width()const;

    u32 Height()const;

    Vector2u Size()const;

    bool IsEmpty()const;
};

SX_INLINE void *Image::Data()const{
    return m_Data;
}

SX_INLINE u32 Image::Width()const{
    return m_Width;
}

SX_INLINE u32 Image::Height()const{
    return m_Height;
}

SX_INLINE Vector2u Image::Size()const{
    return {Width(), Height()};
}

SX_INLINE bool Image::IsEmpty()const{
    return m_Data == nullptr;
}

#endif//STRAITX_IMAGE_HPP