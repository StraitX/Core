#ifndef STRAITX_VULKAN_SHADER_HPP
#define STRAITX_VULKAN_SHADER_HPP


#include "platform/vulkan.hpp"
#include "platform/result.hpp"
#include "graphics/vulkan/logical_device.hpp"

namespace StraitX{
namespace Vk{



struct Shader{
    VkShaderModule Handle = VK_NULL_HANDLE;
    VkPipelineShaderStageCreateInfo Info;
    Vk::LogicalDevice *Owner = nullptr;

    Result Create(Vk::LogicalDevice *owner, const char *filename, VkShaderStageFlagBits stage, VkSpecializationInfo *specialization);

    void Destroy();
};


};//namespace Vk::
};//namespace StraitX::

#endif //STRAITX_VULKAN_SHADER_HPP