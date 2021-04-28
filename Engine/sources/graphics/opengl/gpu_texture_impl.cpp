#include "platform/opengl.hpp"
#include "graphics/opengl/debug.hpp"
#include "graphics/opengl/gpu_texture_impl.hpp"
#include "graphics/opengl/graphics_api_impl.hpp"

namespace StraitX{
namespace GL{

GLint InternalFormatTable[]={
    0,
    GL_RGBA8,
    GL_DEPTH_STENCIL,
    GL_BGRA
};

GLenum GPUTextureImpl::s_FormatTable[] = {
    0,
    GL_RGBA,
    GL_DEPTH_STENCIL,
    GL_BGRA
};

GLenum GPUTextureImpl::s_TypeTable[] = {
    0,
    GL_UNSIGNED_BYTE,
    GL_UNSIGNED_BYTE,
    GL_UNSIGNED_BYTE
};

void GPUTextureImpl::NewImpl(GPUTexture &texture, TextureFormat format, GPUTexture::Usage usage, u32 width, u32 height){
    CoreAssert(format != TextureFormat::Unknown,"GPUTexture: Can't be created with Format::Unknown");

    texture.m_Width = width;
    texture.m_Height = height;
    texture.m_Layout = GPUTexture::Layout::Undefined;
    texture.m_Format = format;
    texture.m_Usage = usage;

    glGenTextures(1, &texture.m_Handle.U32);
    BindZero(texture);
    GL(glTexImage2D(GL_TEXTURE_2D, 0, InternalFormatTable[(u32)format], texture.m_Width, texture.m_Height, 0, s_FormatTable[(u32)format], s_TypeTable[(u32)format], nullptr));
}

void GPUTextureImpl::DeleteImpl(GPUTexture &texture){
    glDeleteTextures(1, &texture.m_Handle.U32);
}

void GPUTextureImpl::BindZero(const GPUTexture &texture){
    GL(glActiveTexture(GL_TEXTURE0 + GraphicsAPIImpl::Instance.MaxTextureUnits() - 1));
    GL(glBindTexture(GL_TEXTURE_2D, texture.m_Handle.U32));
}

}//namespace GL::
}//namespace StraitX::