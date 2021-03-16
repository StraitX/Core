#include "platform/vulkan.hpp"
#include "core/assert.hpp"
#include "graphics/vulkan/logical_gpu_impl.hpp"
#include "graphics/vulkan/sampler_impl.hpp"

namespace StraitX{
namespace Vk{

VkFilter FilteringModeTable[]={
    VK_FILTER_LINEAR,
    VK_FILTER_NEAREST
};

VkSamplerAddressMode WrapModeTable[]={
    VK_SAMPLER_ADDRESS_MODE_REPEAT,
    VK_SAMPLER_ADDRESS_MODE_MIRRORED_REPEAT,
    VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE,
    VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER
};

void SamplerImpl::NewImpl(Sampler &sampler, SamplerProperties props){
    auto *device = static_cast<Vk::LogicalGPUImpl*>(sampler.m_Owner);
    auto &handle = reinterpret_cast<VkSampler&>(sampler.m_Handle.U64);

    VkSamplerCreateInfo info;
    info.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
    info.pNext = nullptr;
    info.flags = 0;
    info.magFilter = FilteringModeTable[(size_t)props.MagFiltering];
    info.minFilter = FilteringModeTable[(size_t)props.MinFiltering];
    info.addressModeU = WrapModeTable[(size_t)props.WrapU];
    info.addressModeV = WrapModeTable[(size_t)props.WrapV];
    info.addressModeW = WrapModeTable[(size_t)props.WrapS];
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

    CoreFunctionAssert(vkCreateSampler(device->Handle, &info, nullptr, &handle),VK_SUCCESS,"Vk: SamplerImpl: Can't create sampler");
}

void SamplerImpl::DeleteImpl(Sampler &sampler){
    auto *device = static_cast<Vk::LogicalGPUImpl*>(sampler.m_Owner);
    auto &handle = reinterpret_cast<VkSampler&>(sampler.m_Handle.U64);
    
    vkDestroySampler(device->Handle, handle, nullptr);
}

}//namespace Vk::
}//namespace StraitX::