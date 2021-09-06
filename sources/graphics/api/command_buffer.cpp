#include "core/os/vulkan.hpp"
#include "graphics/api/graphics_api.hpp"
#include "graphics/api/command_buffer.hpp"
#include "graphics/api/buffer.hpp"

#if defined(SX_VULKAN_SUPPORTED)
    #include "graphics/api/vulkan/command_buffer_impl.hpp"
#endif

CommandPool *CommandPool::Create(){
#if defined(SX_VULKAN_SUPPORTED)
    if(GraphicsAPI::Backend() == GraphicsAPIBackend::Vulkan)
        return new Vk::CommandPoolImpl();
#endif
    return nullptr;
}

void CommandBuffer::Copy(const Buffer *src, const Buffer *dst){
    SX_CORE_ASSERT(src->Size() == dst->Size(), "Buffers should be the same size");

    Copy(src, dst, src->Size(), 0, 0);
}