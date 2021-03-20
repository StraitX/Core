#include "platform/vulkan.hpp"
#include "graphics/vulkan/gpu_buffer_impl.hpp"
#include "graphics/vulkan/logical_gpu_impl.hpp"

namespace StraitX{
namespace Vk{

u32 GetBufferMemoryRequirements(VkDevice owner, VkBuffer buffer){
    VkMemoryRequirements req;
    vkGetBufferMemoryRequirements(owner, buffer, &req);
    return req.size;
}

constexpr GPUBufferImpl::GPUBufferImpl(GPUBuffer &buffer):
    GPUBufferImpl(buffer.m_Owner, reinterpret_cast<VkBuffer&>(buffer.m_Handle.U64), reinterpret_cast<VkDeviceMemory&>(buffer.m_BackingMemory.U64), buffer.m_Size, buffer.m_Usage)
{}

constexpr GPUBufferImpl::GPUBufferImpl(LogicalGPU *&owner, VkBuffer &handle, VkDeviceMemory &memory, u32 &size, GPUBuffer::UsageType &usage):
    Owner(owner),
    Handle(handle),
    Memory(memory),
    Size(size),
    Usage(usage)
{}

void GPUBufferImpl::Create(LogicalGPU &owner, u32 size, GPUMemoryType mem_type, GPUBuffer::UsageType usage){
    Owner = &owner;
    Size = size;
    Usage = usage;

    auto device = static_cast<Vk::LogicalGPUImpl *>(Owner);

    VkBufferCreateInfo info;
    info.sType                  = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    info.pNext                  = nullptr;
    info.flags                  = 0;
    info.sharingMode            = VK_SHARING_MODE_EXCLUSIVE;
    info.queueFamilyIndexCount  = 0;
    info.pQueueFamilyIndices    = nullptr;
    info.size                   = Size;
    info.usage                  = usage;

    CoreFunctionAssert(vkCreateBuffer(device->Handle, &info, nullptr, &Handle), VK_SUCCESS, "Vk: GPUBufferImpl: Can't create buffer");

    Memory = device->Alloc(GetBufferMemoryRequirements(device->Handle, Handle), (mem_type == GPUMemoryType::VRAM) ? MemoryTypes::VRAM : MemoryTypes::DynamicVRAM);

    CoreFunctionAssert(vkBindBufferMemory(device->Handle, Handle, Memory, 0),VK_SUCCESS, "GPUBuffer: can't bind buffer's memory");
}

void GPUBufferImpl::Destroy(){
    auto device = static_cast<Vk::LogicalGPUImpl *>(Owner);

    vkDestroyBuffer(device->Handle, Handle, nullptr);

    device->Free(Memory);
}

// buffer got his owner in constructor
void GPUBufferImpl::NewImpl(GPUBuffer &buffer, LogicalGPU &owner, u32 size, GPUMemoryType mem_type, GPUBuffer::UsageType usage){
    GPUBufferImpl(buffer).Create(owner, size, mem_type, usage);
}

void GPUBufferImpl::DeleteImpl(GPUBuffer &buffer){
    GPUBufferImpl(buffer).Destroy();
}

}//namespace Vk::
}//namespace StraitX::