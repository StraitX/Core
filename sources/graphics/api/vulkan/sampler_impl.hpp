#ifndef STRAITX_VULKAN_SAMPLER_IMPL_HPP
#define STRAITX_VULKAN_SAMPLER_IMPL_HPP

#include "core/os/vulkan.hpp"
#include "graphics/api/sampler.hpp"

namespace Vk{

class SamplerImpl: public Sampler{
private:
    VkSampler m_Handle = VK_NULL_HANDLE;
    SamplerProperties m_Properties;
public:
    SamplerImpl(const SamplerProperties &props);

    ~SamplerImpl();

    SamplerProperties Properties()const override;

    operator VkSampler()const{
        return m_Handle;
    }
};

}//namespace Vk::

#endif//STRAITX_VULKAN_SAMPLER_IMPL_HPP