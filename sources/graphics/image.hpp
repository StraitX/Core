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

    void Clear();

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
    
    u32 &operator[](size_t index);

    const u32 &operator[](size_t index)const ;

    Color Get(size_t x, size_t y);

    void Set(const Color &color, size_t x, size_t y);
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

SX_INLINE u32& Image::operator[](size_t index)
{
    return const_cast<u32&>(const_cast<const Image*>(this)->operator[](index));
}

SX_INLINE const u32& Image::operator[](size_t index) const
{
    SX_CORE_ASSERT(index < Width() * Height(), "Image: index is out of range");
    return m_Data[index];
}

SX_INLINE Color Image::Get(size_t x, size_t y)
{
    return Color(operator[](x + y * Width()));
}

SX_INLINE void Image::Set(const Color& color, size_t x, size_t y)
{
    // we use here ABGR because of little-endian cpu assumption 
    operator[](x + y * Width()) = color.RGBA8();
}

#endif//STRAITX_IMAGE_HPP