#include "graphics/vulkan/cpu_buffer_impl.hpp"
#include "graphics/vulkan/logical_gpu_impl.hpp"
namespace StraitX{
namespace Vk{

void CPUBufferImpl::CreateDedicated(size_t size){
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

    CoreAssert(Memory, "Vk: GPUBuffer: Can't allocate memory");

    CoreFunctionAssert(vkBindBufferMemory(Owner->Handle, Handle, Memory, 0),VK_SUCCESS, "GPUBuffer: can't bind buffer's memory");
}

void CPUBufferImpl::DestroyDedicated(){
    vkDestroyBuffer(Owner->Handle, Handle, nullptr);
    
    Owner->Free(Memory);
}

void CPUBufferImpl::CreateUniform(size_t size){
    Size = size;
    Pointer = Memory::Alloc(size);
}

void CPUBufferImpl::DestroyUniform(){
    Memory::Free(Pointer); 
}

void CPUBufferImpl::NewDedicatedImpl(CPUBuffer &buffer, size_t size){
    CPUBufferImpl impl(buffer);
    impl.CreateDedicated(size);
}

void CPUBufferImpl::DeleteDedicatedImpl(CPUBuffer &buffer){
    CPUBufferImpl impl(buffer);
    impl.DestroyDedicated();
}

void CPUBufferImpl::NewUniformImpl(CPUBuffer &buffer, size_t size){
    CPUBufferImpl impl(buffer);
    impl.CreateUniform(size);
}

void CPUBufferImpl::DeleteUniformImpl(CPUBuffer &buffer){
    CPUBufferImpl impl(buffer);
    impl.DestroyUniform();
}

}//namespace Vk::
}//namespace StraitX::