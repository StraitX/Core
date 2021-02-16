#include "graphics/vulkan/cpu_buffer_impl.hpp"
#include "graphics/vulkan/logical_gpu_impl.hpp"
namespace StraitX{
namespace Vk{

void CPUBufferImpl::Create(u32 size){
    Size = size;

    VkBufferCreateInfo info;
    info.sType                  = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    info.pNext                  = nullptr;
    info.flags                  = 0;
    info.sharingMode            = VK_SHARING_MODE_EXCLUSIVE;
    info.queueFamilyIndexCount  = 0;
    info.pQueueFamilyIndices    = nullptr;
    info.size                   = Size;
    info.usage                  = VK_BUFFER_USAGE_TRANSFER_SRC_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT;

    CoreFunctionAssert(vkCreateBuffer(Owner->Handle, &info, nullptr, &Handle), VK_SUCCESS, "Vk: CPUBufferImpl: Can't create buffer");

    VkMemoryRequirements req;
    vkGetBufferMemoryRequirements(Owner->Handle, Handle, &req);


    Memory = Owner->Alloc(req.size, MemoryTypes::RAM);
    vkMapMemory(Owner->Handle, Memory, 0, req.size, 0, &Pointer);

    CoreAssert(Memory, "Vk: GPUBuffer: Can't allocate memory");

    CoreFunctionAssert(vkBindBufferMemory(Owner->Handle, Handle, Memory, 0),VK_SUCCESS, "GPUBuffer: can't bind buffer's memory");
}

void CPUBufferImpl::Destroy(){
    vkDestroyBuffer(Owner->Handle, Handle, nullptr);

    vkUnmapMemory(Owner->Handle, Memory);
    Owner->Free(Memory);
}

void CPUBufferImpl::NewImpl(CPUBuffer &buffer, u32 size){
    CPUBufferImpl impl(buffer);
    impl.Create(size);
}

void CPUBufferImpl::DeleteImpl(CPUBuffer &buffer){
    CPUBufferImpl impl(buffer);
    impl.Destroy();
}

}//namespace Vk::
}//namespace StraitX::