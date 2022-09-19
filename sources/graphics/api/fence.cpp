#include "core/os/vulkan.hpp"
#include "graphics/api/fence.hpp"
#include "graphics/api/graphics_api.hpp"

#if defined(SX_VULKAN_SUPPORTED)
    #include "graphics/api/vulkan/fence_impl.hpp"
#endif

const Fence::NullFence Fence::Null;

void Fence::WaitFor(){
#if defined(SX_VULKAN_SUPPORTED)
    if(GraphicsAPI::Backend() == GraphicsAPIBackend::Vulkan)
        Vk::FenceImpl::WaitFor(m_Handle);
#endif
}

void Fence::Reset(){
#if defined(SX_VULKAN_SUPPORTED)
    if(GraphicsAPI::Backend() == GraphicsAPIBackend::Vulkan)
        Vk::FenceImpl::Reset(m_Handle);
#endif
}

bool Fence::IsSignaled() const {
#if defined(SX_VULKAN_SUPPORTED)
    if(GraphicsAPI::Backend() == GraphicsAPIBackend::Vulkan)
        return Vk::FenceImpl::IsSignaled(m_Handle);
#endif
    return false;
}

void Fence::Signal(){
#if defined(SX_VULKAN_SUPPORTED)
    if(GraphicsAPI::Backend() == GraphicsAPIBackend::Vulkan)
        Vk::FenceImpl::Signal(m_Handle);
#endif
}

void Fence::Create(){
#if defined(SX_VULKAN_SUPPORTED)
    if(GraphicsAPI::Backend() == GraphicsAPIBackend::Vulkan)
        m_Handle = Vk::FenceImpl::Create();
#endif
}

void Fence::Destroy(){
#if defined(SX_VULKAN_SUPPORTED)
    if(GraphicsAPI::Backend() == GraphicsAPIBackend::Vulkan)
        Vk::FenceImpl::Destroy(m_Handle);
#endif
}