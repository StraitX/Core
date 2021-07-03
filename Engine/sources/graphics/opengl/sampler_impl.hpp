#ifndef STRAITX_OPENGL_SAMPLER_IMPL_HPP
#define STRAITX_OPENGL_SAMPLER_IMPL_HPP

#include "graphics/api/sampler.hpp"

namespace GL{

struct SamplerImpl{
    static void NewImpl(Sampler &sampler, SamplerProperties props);

    static void DeleteImpl(Sampler &sampler);
};

}//namespace GL::

#endif//STRAITX_OPENGL_SAMPLER_IMPL_HPP