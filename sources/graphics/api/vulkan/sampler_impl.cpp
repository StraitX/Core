#include "core/os/vulkan.hpp"
#include "core/assert.hpp"
#include "graphics/api/vulkan/sampler_impl.hpp"
#include "graphics/api/vulkan/debug.hpp"
#include "graphics/api/vulkan/gpu_impl.hpp"

namespace Vk{

const VkFilter s_FilteringModeTable[]={
    VK_FILTER_LINEAR,
    VK_FILTER_NEAREST
};

const VkSamplerAddressMode s_WrapModeTable[]={
    VK_SAMPLER_ADDRESS_MODE_REPEAT,
    VK_SAMPLER_ADDRESS_MODE_MIRRORED_REPEAT,
    VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE,
    VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER
};

SamplerImpl::SamplerImpl(const SamplerProperties &props):
    m_Properties(props)
{
    VkSamplerCreateInfo info;
    info.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
    info.pNext = nullptr;
    info.flags = 0;
    info.magFilter = s_FilteringModeTable[(size_t)props.MagFiltering];
    info.minFilter = s_FilteringModeTable[(size_t)props.MinFiltering];
    info.addressModeU = s_WrapModeTable[(size_t)props.WrapU];
    info.addressModeV = s_WrapModeTable[(size_t)props.WrapV];
    info.addressModeW = s_WrapModeTable[(size_t)props.WrapS];
    info.anisotropyEnable = VK_FALSE;
    info.maxAnisotropy = 0.0; //disabled
    info.compareEnable = VK_FALSE;
    info.compareOp = VK_COMPARE_OP_ALWAYS;//disabled
    info.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
    info.mipLodBias = 0;
    info.minLod = 0;
    info.maxLod = 0;
    info.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
    info.unnormalizedCoordinates = VK_FALSE;

    vkCreateSampler(GPUImpl::s_Instance, &info, nullptr, &m_Handle);
}

SamplerImpl::~SamplerImpl(){
    vkDestroySampler(GPUImpl::s_Instance, m_Handle, nullptr);
}

SamplerProperties SamplerImpl::Properties()const{
    return m_Properties;
}

}//namespace Vk::