#include "core/os/vulkan.hpp"
#include "graphics/api/semaphore.hpp"
#include "graphics/api/graphics_api.hpp"

#if defined(SX_VULKAN_SUPPORTED)
    #include "graphics/api/vulkan/semaphore_impl.hpp"
#endif

const Semaphore Semaphore::Null(nullptr);

void Semaphore::WaitFor()const{
#if defined(SX_VULKAN_SUPPORTED)
    if(GraphicsAPI::Backend() == GraphicsAPIBackend::Vulkan)
        Vk::SemaphoreImpl::WaitFor(*this);
#endif
}

void Semaphore::Create(){
#if defined(SX_VULKAN_SUPPORTED)
    if(GraphicsAPI::Backend() == GraphicsAPIBackend::Vulkan)
        m_Handle = Vk::SemaphoreImpl::Create();
#endif
}

void Semaphore::Destroy(){
#if defined(SX_VULKAN_SUPPORTED)
    if(GraphicsAPI::Backend() == GraphicsAPIBackend::Vulkan)
        Vk::SemaphoreImpl::Destroy(m_Handle);
#endif
}