#include "graphics/api/buffer.hpp"
#include "graphics/api/graphics_api.hpp"
#include "core/os/vulkan.hpp"

#ifdef SX_VULKAN_SUPPORTED
    #include "graphics/api/vulkan/buffer_impl.hpp"
#endif

Buffer *Buffer::Create(size_t size, BufferMemoryType mem_type, BufferUsage usage){
#ifdef SX_VULKAN_SUPPORTED
    if(GraphicsAPI::Backend() == GraphicsAPIBackend::Vulkan)
        return new Vk::BufferImpl(size, mem_type, usage);
#endif
    return nullptr;
}