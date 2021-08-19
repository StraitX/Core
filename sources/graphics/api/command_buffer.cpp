#include "core/os/vulkan.hpp"
#include "graphics/api/graphics_api.hpp"
#include "graphics/api/command_buffer.hpp"

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