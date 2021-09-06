#include "core/os/memory.hpp"
#include "graphics/api/vulkan/buffer_impl.hpp"
#include "graphics/api/vulkan/memory_allocator.hpp"
#include "graphics/api/vulkan/debug.hpp"

namespace Vk{

BufferImpl::BufferImpl(size_t size, BufferMemoryType mem_type, BufferUsage usage):
    m_Size(size),
    m_MemoryType(mem_type),
    m_RealMemoryType(GPUImpl::s_Instance.ReadMemoryType(MemoryType::Type(mem_type)))
{
    VkBufferCreateInfo info;
    info.sType                  = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    info.pNext                  = nullptr;
    info.flags                  = 0;
    info.sharingMode            = VK_SHARING_MODE_EXCLUSIVE;
    info.queueFamilyIndexCount  = 0;
    info.pQueueFamilyIndices    = nullptr;
    info.size                   = size;
    info.usage                  = usage;

    SX_VK_ASSERT(vkCreateBuffer(GPUImpl::s_Instance, &info, nullptr, &m_Handle), "Vk: GPUBufferImpl: Can't create buffer");

    VkMemoryRequirements req;
    vkGetBufferMemoryRequirements(GPUImpl::s_Instance, m_Handle, &req);

    m_Memory = MemoryAllocator::Alloc(req.size, MemoryType::Type(mem_type));

    SX_VK_ASSERT(vkBindBufferMemory(GPUImpl::s_Instance, m_Handle, m_Memory, 0), "GPUBuffer: can't bind buffer's memory");
}

BufferImpl::~BufferImpl(){
    if(IsMapped())
        Unmap();

    MemoryAllocator::Free(m_Memory);

    vkDestroyBuffer(GPUImpl::s_Instance, m_Handle, nullptr);
}

void BufferImpl::Copy(const void *data, size_t size, size_t offset){
    if(IsMappable()){
        if(!IsMapped())Map();

        Memory::Copy(data, (u8*)m_Pointer + offset, size);
    }else{
        SX_ASSERT(false, "DMA is not implemented");
    }
}

bool BufferImpl::IsMappable()const{
    return m_RealMemoryType == Vk::MemoryType::DynamicVRAM
        || m_RealMemoryType == Vk::MemoryType::RAM
        || m_RealMemoryType == Vk::MemoryType::UncachedRAM;
}

bool BufferImpl::IsMapped()const{
    return m_Pointer != nullptr;
}

void *BufferImpl::Map(){
    vkMapMemory(GPUImpl::s_Instance, m_Memory, 0, m_Size, 0, &m_Pointer);
    return m_Pointer;
}

void BufferImpl::Unmap(){
    vkUnmapMemory(GPUImpl::s_Instance, m_Memory);
    m_Pointer = nullptr;
}

size_t BufferImpl::Size()const{
    return m_Size;
}

BufferMemoryType BufferImpl::MemoryType()const{
    return m_MemoryType;
}

BufferUsage BufferImpl::Usage()const{
    return m_Usage;
}

}//namespace Vk::
