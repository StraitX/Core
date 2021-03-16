#include "platform/opengl.hpp"
#include "graphics/opengl/sampler_impl.hpp"

namespace StraitX{
namespace GL{

static GLenum FilteringTable[]={
    GL_LINEAR,
    GL_NEAREST
};

static GLenum WrapModeTable[]={
    GL_REPEAT,
    GL_MIRRORED_REPEAT,
    GL_CLAMP_TO_EDGE,
    GL_CLAMP_TO_BORDER
};

void SamplerImpl::NewImpl(Sampler &sampler, SamplerProperties props){
    glGenSamplers(1,&sampler.m_Handle.U32);
    glSamplerParameteri(sampler.m_Handle.U32, GL_TEXTURE_MIN_FILTER, FilteringTable[(size_t)props.MinFiltering]);
    glSamplerParameteri(sampler.m_Handle.U32, GL_TEXTURE_MAG_FILTER, FilteringTable[(size_t)props.MagFiltering]);
    glSamplerParameteri(sampler.m_Handle.U32, GL_TEXTURE_WRAP_S, WrapModeTable[(size_t)props.WrapU]);
    glSamplerParameteri(sampler.m_Handle.U32, GL_TEXTURE_WRAP_T, WrapModeTable[(size_t)props.WrapV]);
    glSamplerParameteri(sampler.m_Handle.U32, GL_TEXTURE_WRAP_R, WrapModeTable[(size_t)props.WrapS]);
}

void SamplerImpl::DeleteImpl(Sampler &sampler){
    glDeleteSamplers(1, &sampler.m_Handle.U32);
}

}//namespace GL::
}//namespace StraitX::
