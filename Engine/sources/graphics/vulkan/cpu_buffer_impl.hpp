#ifndef STRAITX_VULKAN_CPU_BUFFER_IMPL_HPP
#define STRAITX_VULKAN_CPU_BUFFER_IMPL_HPP

#include "platform/vulkan.hpp"
#include "graphics/api/cpu_buffer.hpp"
#include "graphics/vulkan/logical_gpu_impl.hpp"

namespace StraitX{
namespace Vk{

struct CPUBufferImpl{
    LogicalGPU *&Owner;
    VkBuffer &Handle;
    VkDeviceMemory &Memory;
    u32 &Size;
    void *&Pointer;

    // NOTE: CPUBuffer should already have an owner because we can't cast type of pointer in a reference to pointer variable
    constexpr CPUBufferImpl(CPUBuffer &buffer):
        CPUBufferImpl(
            buffer.m_Owner, 
            reinterpret_cast<VkBuffer&>(buffer.m_Handle.U64),
            reinterpret_cast<VkDeviceMemory&>(buffer.m_BackingMemory.U64), 
            buffer.m_Size, 
            buffer.m_Pointer)
    {}

    constexpr CPUBufferImpl(LogicalGPU *&owner, VkBuffer &handle, VkDeviceMemory &memory, u32 &size, void *&pointer):
        Owner(owner),
        Handle(handle),
        Memory(memory),
        Size(size),
        Pointer(pointer)
    {}

    sx_inline void Create(LogicalGPU *owner, u32 size);

    sx_inline void Destroy();

    static void NewImpl(CPUBuffer &buffer, LogicalGPU &owner, u32 size);

    static void DeleteImpl(CPUBuffer &buffer);
};

}//namespace Vk::
}//namespace StraitX::

#endif//STRAITX_VULKAN_CPU_BUFFER_IMPL_HPP