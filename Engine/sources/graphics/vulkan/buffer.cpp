#include "graphics/vulkan/buffer.hpp"


namespace StraitX{
namespace Vk{


Result Buffer::Create(Vk::LogicalDevice *owner, u32 size, VkBufferUsageFlags usage){
    Owner = owner;
    Size = size;

    VkBufferCreateInfo info;
    info.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    info.pNext = nullptr;
    info.flags = 0;
    info.size = Size;
    info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    info.queueFamilyIndexCount = 0;
    info.pQueueFamilyIndices = nullptr;
    info.usage = usage;

    return ResultError(vkCreateBuffer(Owner->Handle, &info, nullptr, &Handle) != VK_SUCCESS);
}

void Buffer::Destroy(){
    vkDestroyBuffer(Owner->Handle, Handle, nullptr);
}


};//namespace Vk::
};//namespace StraitX::