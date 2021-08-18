#include "graphics/api/gpu.hpp"
#include "graphics/api/graphics_api.hpp"
#include "core/assert.hpp"

#include "core/os/vulkan.hpp"

#if defined(SX_VULKAN_SUPPORTED)
    #include "graphics/api/vulkan/gpu_impl.hpp"
#endif


GPUImpl *GPU::s_Impl = nullptr;

Result GPU::Initialize(){
    SX_CORE_ASSERT(GraphicsAPI::HasBackend(), "Can't initialize GPU without GraphicsAPI Backend");

#if defined(SX_VULKAN_SUPPORTED)
    if(GraphicsAPI::Backend() == GraphicsAPIBackend::Vulkan)
        s_Impl = &Vk::GPUImpl::s_Instance;
#endif

    if(s_Impl)
        return s_Impl->Initialize();
    return Result::Unavailable;
}

void GPU::Finalize(){
    SX_CORE_ASSERT(GraphicsAPI::HasBackend(), "Can't finalize GPU without GraphicsAPI Backend");

    if(s_Impl)
        s_Impl->Finalize();
    s_Impl = nullptr;
}

GPUType GPU::Type(){
    SX_CORE_ASSERT(IsInitialized(), "GPU Is not initalized");

    return s_Impl->Type;
}

GPUVendor GPU::Vendor(){
    SX_CORE_ASSERT(IsInitialized(), "GPU Is not initalized");
    
    return s_Impl->Vendor;
}