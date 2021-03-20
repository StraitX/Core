#ifndef STRAITX_OPENGL_GPU_TEXTURE_IMPL_HPP
#define STRAITX_OPENGL_GPU_TEXTURE_IMPL_HPP

#include "platform/opengl.hpp"
#include "graphics/api/gpu_texture.hpp"

namespace StraitX{
namespace GL{

struct GPUTextureImpl{
    static GLenum s_FormatTable[];
    static GLenum s_TypeTable[];

    static void NewImpl(GPUTexture &texture, LogicalGPU &owner, TextureFormat format, GPUTexture::Usage usage, u32 width, u32 height);

    static void DeleteImpl(GPUTexture &texture);

    static void BindZero(const GPUTexture &texture);
};

}//namespace GL::
}//namespace StraitX::

#endif//STRAITX_OPENGL_GPU_TEXTURE_IMPL_HPP