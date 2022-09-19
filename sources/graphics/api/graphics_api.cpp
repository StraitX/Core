#include "graphics/api/graphics_api.hpp"
#include "core/os/vulkan.hpp"

#if defined(SX_VULKAN_SUPPORTED)
    #include "graphics/api/vulkan/graphics_api_impl.hpp"
#endif

GraphicsAPIBackendImpl *GraphicsAPI::s_Impl = nullptr;
GraphicsAPIBackend GraphicsAPI::s_Backend = GraphicsAPIBackend::Vulkan;

Result GraphicsAPI::CreateBackend(){
#if defined(SX_VULKAN_SUPPORTED)
    if(s_Backend == GraphicsAPIBackend::Vulkan)
        s_Impl = &Vk::GraphicsAPIBackendImpl::Get();
#endif

    if(s_Impl){
        return s_Impl->Create();
    }
    
    return Result::Unsupported;
}

void GraphicsAPI::DestroyBackend(){
    if(s_Impl)
        s_Impl->Destroy();
    s_Impl = nullptr;
}