#include "platform/vulkan.hpp"
#include "graphics/vulkan/gpu_buffer_impl.hpp"
#include "graphics/vulkan/logical_gpu_impl.hpp"

namespace StraitX{
namespace Vk{

sx_inline u32 GPUBufferImpl::GetBufferMemoryRequirements(const GPUBuffer &buffer){
    LogicalGPUImpl &buffer_owner_ref  = *static_cast<LogicalGPUImpl*>(buffer.m_Owner);
    const VkBuffer &buffer_handle_ref = reinterpret_cast<const VkBuffer&>(buffer.m_Handle.U64);

    VkMemoryRequirements req;
    vkGetBufferMemoryRequirements(buffer_owner_ref.Handle, buffer_handle_ref, &req);

    return req.size;
}

sx_inline Result GPUBufferImpl::AcquireMemory(GPUBuffer &buffer, GPUMemoryType mem_type){
    LogicalGPUImpl &buffer_owner_ref  = *static_cast<LogicalGPUImpl*>(buffer.m_Owner);
    VkBuffer       &buffer_handle_ref = reinterpret_cast<VkBuffer&>(buffer.m_Handle.U64);
    VkDeviceMemory &buffer_memory_ref = reinterpret_cast<VkDeviceMemory&>(buffer.m_BackingMemory.U64);

    buffer_memory_ref = buffer_owner_ref.Alloc(GetBufferMemoryRequirements(buffer), mem_type);
    if(buffer_memory_ref == VK_NULL_HANDLE)
        return Result::MemoryFailure;

    return ResultError(vkBindBufferMemory(buffer_owner_ref.Handle, buffer_handle_ref, buffer_memory_ref, 0) != VK_SUCCESS);
}

sx_inline void GPUBufferImpl::ReleaseMemory(GPUBuffer &buffer){
    LogicalGPUImpl &buffer_owner_ref  = *static_cast<LogicalGPUImpl*>(buffer.m_Owner);
    VkDeviceMemory &buffer_memory_ref = reinterpret_cast<VkDeviceMemory&>(buffer.m_BackingMemory.U64);
    
    buffer_owner_ref.Free(buffer_memory_ref);
}

Result GPUBufferImpl::Create(GPUBuffer &buffer, LogicalGPU &owner, u32 size, GPUMemoryType mem_type, GPUBuffer::UsageType usage){
    buffer.m_Owner = &owner;
    buffer.m_Size  = size;

    VkBufferCreateInfo info;
    info.sType                  = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    info.pNext                  = nullptr;
    info.flags                  = 0;
    info.sharingMode            = VK_SHARING_MODE_EXCLUSIVE;
    info.queueFamilyIndexCount  = 0;
    info.pQueueFamilyIndices    = nullptr;
    info.size                   = buffer.m_Size;
    info.usage                  = usage;

    LogicalGPUImpl &buffer_owner_ref  = *static_cast<LogicalGPUImpl*>(buffer.m_Owner);
    VkBuffer       &buffer_handle_ref = reinterpret_cast<VkBuffer&>(buffer.m_Handle.U64);

    if(vkCreateBuffer(buffer_owner_ref.Handle, &info, nullptr, &buffer_handle_ref) != VK_SUCCESS)
        return Result::Failure;

    return AcquireMemory(buffer, mem_type);
}

void GPUBufferImpl::Destroy(GPUBuffer &buffer){
    ReleaseMemory(buffer);

    LogicalGPUImpl &buffer_owner_ref  = *static_cast<LogicalGPUImpl*>(buffer.m_Owner);
    VkBuffer       &buffer_handle_ref = reinterpret_cast<VkBuffer&>(buffer.m_Handle.U64);

    vkDestroyBuffer(buffer_owner_ref.Handle, buffer_handle_ref, nullptr);

    buffer.m_Owner = nullptr;
    buffer.m_Handle.U64 = 0;
    buffer.m_Size = 0;
    buffer.m_BackingMemory.U64 = 0;
}

}//namespace Vk::
}//namespace StraitX::