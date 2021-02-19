#include "platform/vulkan.hpp"
#include "graphics/vulkan/gpu_buffer_impl.hpp"
#include "graphics/vulkan/logical_gpu_impl.hpp"

namespace StraitX{
namespace Vk{

constexpr GPUBufferImpl::GPUBufferImpl(GPUBuffer &buffer):
    GPUBufferImpl(static_cast<LogicalGPUImpl*>(buffer.m_Owner), reinterpret_cast<VkBuffer&>(buffer.m_Handle.U64), reinterpret_cast<VkDeviceMemory&>(buffer.m_BackingMemory.U64), buffer.m_Size)
{}

constexpr GPUBufferImpl::GPUBufferImpl(Vk::LogicalGPUImpl *const owner, VkBuffer &handle, VkDeviceMemory &memory, u32 &size):
    Owner(owner),
    Handle(handle),
    Memory(memory),
    Size(size)
{}

void GPUBufferImpl::Create(u32 size, GPUMemoryType mem_type, GPUBuffer::UsageType usage){
    Size = size;

    VkBufferCreateInfo info;
    info.sType                  = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    info.pNext                  = nullptr;
    info.flags                  = 0;
    info.sharingMode            = VK_SHARING_MODE_EXCLUSIVE;
    info.queueFamilyIndexCount  = 0;
    info.pQueueFamilyIndices    = nullptr;
    info.size                   = Size;
    info.usage                  = usage;

    CoreFunctionAssert(vkCreateBuffer(Owner->Handle, &info, nullptr, &Handle), VK_SUCCESS, "Vk: GPUBufferImpl: Can't create buffer");

    Memory = Owner->Alloc(GetBufferMemoryRequirements(Owner->Handle, Handle), (mem_type == GPUMemoryType::VRAM) ? MemoryTypes::VRAM : MemoryTypes::DynamicVRAM);

    CoreFunctionAssert(vkBindBufferMemory(Owner->Handle, Handle, Memory, 0),VK_SUCCESS, "GPUBuffer: can't bind buffer's memory");
}

void GPUBufferImpl::Destroy(){
    vkDestroyBuffer(Owner->Handle, Handle, nullptr);

    Owner->Free(Memory);
}

u32 GPUBufferImpl::GetBufferMemoryRequirements(VkDevice owner, VkBuffer buffer){
    VkMemoryRequirements req;
    vkGetBufferMemoryRequirements(owner, buffer, &req);
    return req.size;
}

// buffer got his owner in constructor
void GPUBufferImpl::NewImpl(GPUBuffer &buffer, u32 size, GPUMemoryType mem_type, GPUBuffer::UsageType usage){
    GPUBufferImpl impl(buffer);
    impl.Create(size, mem_type, usage);
}

void GPUBufferImpl::DeleteImpl(GPUBuffer &buffer){
    GPUBufferImpl impl(buffer);
    impl.Destroy();
}

}//namespace Vk::
}//namespace StraitX::