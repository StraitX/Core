#ifndef STRAITX_OPENGL_GPU_TEXTURE_IMPL_HPP
#define STRAITX_OPENGL_GPU_TEXTURE_IMPL_HPP

#include "platform/opengl.hpp"
#include "graphics/api/texture.hpp"

namespace GL{

struct TextureImpl{
    static GLenum s_FormatTable[];
    static GLenum s_TypeTable[];
};

struct Texture2DImpl{
    static void NewImpl(Texture2D &texture, u32 width, u32 height, TextureFormat format, TextureUsageBits usage);

    static void DeleteImpl(Texture2D &texture);

    static void BindZero(const Texture2D &texture);
};

}//namespace GL::

#endif//STRAITX_OPENGL_GPU_TEXTURE_IMPL_HPP