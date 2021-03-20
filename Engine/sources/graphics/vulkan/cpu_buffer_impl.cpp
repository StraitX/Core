#include "graphics/vulkan/cpu_buffer_impl.hpp"
#include "graphics/vulkan/logical_gpu_impl.hpp"
namespace StraitX{
namespace Vk{

void CPUBufferImpl::Create(LogicalGPU *owner, u32 size){
    Size = size;
    Owner = owner;

    auto device = static_cast<Vk::LogicalGPUImpl *>(Owner);

    VkBufferCreateInfo info;
    info.sType                  = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    info.pNext                  = nullptr;
    info.flags                  = 0;
    info.sharingMode            = VK_SHARING_MODE_EXCLUSIVE;
    info.queueFamilyIndexCount  = 0;
    info.pQueueFamilyIndices    = nullptr;
    info.size                   = Size;
    info.usage                  = VK_BUFFER_USAGE_TRANSFER_SRC_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT;

    CoreFunctionAssert(vkCreateBuffer(device->Handle, &info, nullptr, &Handle), VK_SUCCESS, "Vk: CPUBufferImpl: Can't create buffer");

    VkMemoryRequirements req;
    vkGetBufferMemoryRequirements(device->Handle, Handle, &req);


    Memory = device->Alloc(req.size, MemoryTypes::RAM);
    vkMapMemory(device->Handle, Memory, 0, req.size, 0, &Pointer);

    CoreAssert(Memory, "Vk: GPUBuffer: Can't allocate memory");

    CoreFunctionAssert(vkBindBufferMemory(device->Handle, Handle, Memory, 0),VK_SUCCESS, "GPUBuffer: can't bind buffer's memory");
}

void CPUBufferImpl::Destroy(){
    auto device = static_cast<Vk::LogicalGPUImpl *>(Owner);

    vkDestroyBuffer(device->Handle, Handle, nullptr);

    vkUnmapMemory(device->Handle, Memory);
    device->Free(Memory);
}

void CPUBufferImpl::NewImpl(CPUBuffer &buffer, LogicalGPU &owner, u32 size){
    CPUBufferImpl(buffer).Create(&owner, size);
}

void CPUBufferImpl::DeleteImpl(CPUBuffer &buffer){
    CPUBufferImpl(buffer).Destroy();
}

}//namespace Vk::
}//namespace StraitX::