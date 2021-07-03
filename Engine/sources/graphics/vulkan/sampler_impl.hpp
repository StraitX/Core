#ifndef STRAITX_VULKAN_SAMPLER_IMPL_HPP
#define STRAITX_VULKAN_SAMPLER_IMPL_HPP

#include "graphics/api/sampler.hpp"

namespace Vk{

struct SamplerImpl{
    static void NewImpl(Sampler &sampler, SamplerProperties props);

    static void DeleteImpl(Sampler &sampler);
};

}//namespace Vk::

#endif//STRAITX_VULKAN_SAMPLER_IMPL_HPP