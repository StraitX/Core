#ifndef STRAITX_VULKAN_SAMPLER_IMPL_HPP
#define STRAITX_VULKAN_SAMPLER_IMPL_HPP

#include "graphics/api/sampler.hpp"

namespace StraitX{
namespace Vk{

struct SamplerImpl{
    static void NewImpl(Sampler &sampler, LogicalGPU &owner, SamplerProperties props);

    static void DeleteImpl(Sampler &sampler);
};

}//namespace Vk::
}//namespace StraitX::

#endif//STRAITX_VULKAN_SAMPLER_IMPL_HPP