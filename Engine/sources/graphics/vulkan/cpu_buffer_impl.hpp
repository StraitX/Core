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
    size_t   &Size;
    void*    &Pointer;

    // NOTE: CPUBuffer should already have an owner because we can't cast type of pointer in a reference to pointer variable
    constexpr CPUBufferImpl(CPUBuffer &buffer):
        CPUBufferImpl(
            static_cast<Vk::LogicalGPUImpl *>(buffer.m_Owner), 
            reinterpret_cast<VkBuffer&>(buffer.m_Handle.U64),
            reinterpret_cast<VkDeviceMemory&>(buffer.m_BackingMemory.U64), 
            buffer.m_Size, 
            buffer.m_Pointer)
    {}

    constexpr CPUBufferImpl(LogicalGPUImpl *owner, VkBuffer &handle, VkDeviceMemory &memory, size_t &size, void *&pointer):
        Owner(owner),
        Handle(handle),
        Memory(memory),
        Size(size),
        Pointer(pointer)
    {}

    sx_inline void CreateDedicated(size_t size);

    sx_inline void DestroyDedicated();

    sx_inline void CreateUniform(size_t size);

    sx_inline void DestroyUniform();

    static void NewDedicatedImpl(CPUBuffer &buffer, size_t size);

    static void DeleteDedicatedImpl(CPUBuffer &buffer);

    static void NewUniformImpl(CPUBuffer &buffer, size_t size);

    static void DeleteUniformImpl(CPUBuffer &buffer);
};

}//namespace Vk::
}//namespace StraitX::

#endif//STRAITX_VULKAN_CPU_BUFFER_IMPL_HPP