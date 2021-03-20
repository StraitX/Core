#ifndef STRAITX_OPENGL_SAMPLER_IMPL_HPP
#define STRAITX_OPENGL_SAMPLER_IMPL_HPP

#include "graphics/api/sampler.hpp"

namespace StraitX{
namespace GL{

struct SamplerImpl{
    static void NewImpl(Sampler &sampler, LogicalGPU &owner, SamplerProperties props);

    static void DeleteImpl(Sampler &sampler);
};

}//namespace GL::
}//namespace StraitX::

#endif//STRAITX_OPENGL_SAMPLER_IMPL_HPP