#include "core/assert.hpp"
#include "graphics/vulkan/fence.hpp"

namespace StraitX{
namespace Vk{

void Fence::Create(Vk::LogicalDevice *owner){
    Owner = owner;
    VkFenceCreateInfo info = {};
    info.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    CoreAssert(vkCreateFence(Owner->Handle, &info, nullptr, &Handle) == VK_SUCCESS, "Fence: Creation failed");
}


};//namespace Vk::
};//namespace StraitX::