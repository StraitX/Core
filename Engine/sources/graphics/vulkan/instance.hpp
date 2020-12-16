#ifndef STRAITX_VULKAN_INSTANCE_HPP
#define STRAITX_VULKAN_INSTANCE_HPP

#include "platform/vulkan.hpp"
#include "platform/error.hpp"
#include "platform/types.hpp"
#include "core/array_ptr.hpp"

namespace StraitX{
namespace Vk{

struct Instance{
    VkInstance Handle = VK_NULL_HANDLE;

    Error Create(const Version &version, const ArrayPtr<char *> &extensions, const ArrayPtr<char *> &layers);

    void Destroy();
};

};//namespace Vk::
};//namespace StraitX::
#endif // STRAITX_VULKAN_INSTANCE_HPP