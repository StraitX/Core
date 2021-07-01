#include "platform/opengl.hpp"
#include "graphics/opengl/debug.hpp"
#include "graphics/opengl/texture_impl.hpp"
#include "graphics/opengl/graphics_context_impl.hpp"

namespace StraitX{
namespace GL{

GLint InternalFormatTable[]={
    0,
    GL_RGBA8,
    GL_DEPTH_STENCIL,
    GL_BGRA,
    GL_DEPTH_STENCIL
};

GLenum TextureImpl::s_FormatTable[] = {
    0,
    GL_RGBA,
    GL_DEPTH_STENCIL,
    GL_BGRA,
    GL_DEPTH_STENCIL // XXX wrong
};

GLenum TextureImpl::s_TypeTable[] = {
    0,
    GL_UNSIGNED_BYTE,
    GL_UNSIGNED_BYTE,
    GL_UNSIGNED_BYTE,
    GL_UNSIGNED_BYTE
};

void Texture2DImpl::NewImpl(Texture2D &texture, u32 width, u32 height, TextureFormat format, TextureUsageBits usage){
    SX_CORE_ASSERT(format != TextureFormat::Unknown,"GPUTexture: Can't be created with Format::Unknown");

    texture.m_Width = width;
    texture.m_Height = height;
    texture.m_Layout = TextureLayout::Undefined;
    texture.m_Format = format;
    texture.m_Usage = usage;

    glGenTextures(1, &texture.m_Handle.U32);
    BindZero(texture);
    GL(glTexImage2D(GL_TEXTURE_2D, 0, InternalFormatTable[(u32)format], texture.m_Width, texture.m_Height, 0, TextureImpl::s_FormatTable[(u32)format], TextureImpl::s_TypeTable[(u32)format], nullptr));
}

void Texture2DImpl::DeleteImpl(Texture2D &texture){
    glDeleteTextures(1, &texture.m_Handle.U32);
}

void Texture2DImpl::BindZero(const Texture2D &texture){
    GL(glActiveTexture(GL_TEXTURE0 + GraphicsContextImpl::s_Instance.MaxTextureUnits() - 1));
    GL(glBindTexture(GL_TEXTURE_2D, texture.m_Handle.U32));
}

}//namespace GL::
}//namespace StraitX::