#include "platform/vulkan.hpp"
#include "graphics/vulkan/gpu_buffer_impl.hpp"
#include "graphics/vulkan/memory_allocator.hpp"
#include "graphics/vulkan/gpu.hpp"

namespace Vk{

GPUBufferImpl::GPUBufferImpl(GPUBuffer &buffer):
    GPUBufferImpl(reinterpret_cast<VkBuffer&>(buffer.m_Handle.U64), reinterpret_cast<VkDeviceMemory&>(buffer.m_BackingMemory.U64), buffer.m_Size, buffer.m_Usage, buffer.m_MemoryType)
{}

GPUBufferImpl::GPUBufferImpl(VkBuffer &handle, VkDeviceMemory &memory, u32 &size, GPUBuffer::UsageType &usage, GPUMemoryType &mem_type):
    Handle(handle),
    Memory(memory),
    Size(size),
    Usage(usage),
    MemoryType(mem_type)
{}

void GPUBufferImpl::Create(u32 size, GPUMemoryType mem_type, GPUBuffer::UsageType usage){
    Size = size;
    Usage = usage;
    MemoryType = mem_type;

    VkBufferCreateInfo info;
    info.sType                  = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    info.pNext                  = nullptr;
    info.flags                  = 0;
    info.sharingMode            = VK_SHARING_MODE_EXCLUSIVE;
    info.queueFamilyIndexCount  = 0;
    info.pQueueFamilyIndices    = nullptr;
    info.size                   = Size;
    info.usage                  = usage;

    CoreFunctionAssert(vkCreateBuffer(GPU::Get().Handle(), &info, nullptr, &Handle), VK_SUCCESS, "Vk: GPUBufferImpl: Can't create buffer");

    VkMemoryRequirements req;
    vkGetBufferMemoryRequirements(GPU::Get().Handle(), Handle, &req);

    Memory = MemoryAllocator::Alloc(req.size, ToVkMemoryType(mem_type));

    CoreFunctionAssert(vkBindBufferMemory(GPU::Get().Handle(), Handle, Memory, 0),VK_SUCCESS, "GPUBuffer: can't bind buffer's memory");
}

void GPUBufferImpl::Destroy(){
    vkDestroyBuffer(GPU::Get().Handle(), Handle, nullptr);

    MemoryAllocator::Free(Memory);
}

// buffer got his owner in constructor
void GPUBufferImpl::NewImpl(GPUBuffer &buffer, u32 size, GPUMemoryType mem_type, GPUBuffer::UsageType usage){
    GPUBufferImpl(buffer).Create(size, mem_type, usage);
}

void GPUBufferImpl::DeleteImpl(GPUBuffer &buffer){
    GPUBufferImpl(buffer).Destroy();
}

}//namespace Vk::