#ifndef STRAITX_VULKAN_INSTANCE_HPP
#define STRAITX_VULKAN_INSTANCE_HPP

#include "platform/vulkan.hpp"
#include "platform/error.hpp"
#include "platform/types.hpp"

namespace StraitX{
namespace Vk{

struct Instance{
    VkInstance Handle = VK_NULL_HANDLE;

    Error Create(const Version &version, const char **extensions, size_t ecount, const char **layers, size_t lcount);

    void Destroy();
};

};//namespace Vk::
};//namespace StraitX::
#endif // STRAITX_VULKAN_INSTANCE_HPP