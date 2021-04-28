#include "graphics/vulkan/cpu_buffer_impl.hpp"
#include "graphics/vulkan/gpu.hpp"
#include "graphics/vulkan/memory_allocator.hpp"

namespace StraitX{
namespace Vk{

CPUBufferImpl::CPUBufferImpl(CPUBuffer &buffer):
    CPUBufferImpl(
        reinterpret_cast<VkBuffer&>(buffer.m_Handle.U64),
        reinterpret_cast<VkDeviceMemory&>(buffer.m_BackingMemory.U64), 
        buffer.m_Size, 
        buffer.m_Pointer)
{}

CPUBufferImpl::CPUBufferImpl(VkBuffer &handle, VkDeviceMemory &memory, u32 &size, void *&pointer):
    Handle(handle),
    Memory(memory),
    Size(size),
    Pointer(pointer)
{}


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

    CoreFunctionAssert(vkCreateBuffer(GPU::Get().Handle(), &info, nullptr, &Handle), VK_SUCCESS, "Vk: CPUBufferImpl: Can't create buffer");

    VkMemoryRequirements req;
    vkGetBufferMemoryRequirements(GPU::Get().Handle(), Handle, &req);

    Memory = MemoryAllocator::Alloc(req.size, MemoryType::RAM);

    vkMapMemory(GPU::Get().Handle(), Memory, 0, req.size, 0, &Pointer);

    CoreFunctionAssert(vkBindBufferMemory(GPU::Get().Handle(), Handle, Memory, 0),VK_SUCCESS, "GPUBuffer: can't bind buffer's memory");
}

void CPUBufferImpl::Destroy(){
    vkDestroyBuffer(GPU::Get().Handle(), Handle, nullptr);

    vkUnmapMemory(GPU::Get().Handle(), Memory);

    MemoryAllocator::Free(Memory);
}

void CPUBufferImpl::NewImpl(CPUBuffer &buffer, u32 size){
    CPUBufferImpl(buffer).Create(size);
}

void CPUBufferImpl::DeleteImpl(CPUBuffer &buffer){
    CPUBufferImpl(buffer).Destroy();
}

}//namespace Vk::
}//namespace StraitX::