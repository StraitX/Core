#ifndef STRAITX_VULKAN_CPU_BUFFER_IMPL_HPP
#define STRAITX_VULKAN_CPU_BUFFER_IMPL_HPP

#include "platform/vulkan.hpp"
#include "graphics/api/cpu_buffer.hpp"
#include "graphics/vulkan/logical_gpu_impl.hpp"

namespace StraitX{
namespace Vk{

struct CPUBufferImpl{
    LogicalGPUImpl * const Owner;
    VkBuffer &Handle;
    VkDeviceMemory &Memory;
    u32 &Size;
    void *&Pointer;

    // NOTE: CPUBuffer should already have an owner because we can't cast type of pointer in a reference to pointer variable
    constexpr CPUBufferImpl(CPUBuffer &buffer):
        CPUBufferImpl(
            static_cast<Vk::LogicalGPUImpl *>(buffer.m_Owner), 
            reinterpret_cast<VkBuffer&>(buffer.m_Handle.U64),
            reinterpret_cast<VkDeviceMemory&>(buffer.m_BackingMemory.U64), 
            buffer.m_Size, 
            buffer.m_Pointer)
    {}

    constexpr CPUBufferImpl(LogicalGPUImpl *owner, VkBuffer &handle, VkDeviceMemory &memory, u32 &size, void *&pointer):
        Owner(owner),
        Handle(handle),
        Memory(memory),
        Size(size),
        Pointer(pointer)
    {}

    sx_inline void Create(u32 size);

    sx_inline void Destroy();

    static void NewImpl(CPUBuffer &buffer, u32 size);

    static void DeleteImpl(CPUBuffer &buffer);
};

}//namespace Vk::
}//namespace StraitX::

#endif//STRAITX_VULKAN_CPU_BUFFER_IMPL_HPP