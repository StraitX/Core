#include "graphics/api/gpu.hpp"
#include "graphics/api/graphics_api.hpp"
#include "core/assert.hpp"
#include "core/os/memory.hpp"

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


void GPU::Execute(CommandBuffer *buffer, Span<u64> wait_semaphore_handles, Span<u64> signal_semaphore_handles, const Fence &signal_fence){
    SX_CORE_ASSERT(IsInitialized(), "GPU Is not initalized");
    
    s_Impl->Execute(buffer, wait_semaphore_handles, signal_semaphore_handles, signal_fence);
}

void GPU::Execute(CommandBuffer *buffer, Span<const Semaphore> wait_semaphores, Span<const Semaphore> signal_semaphores, const Fence &signal_fence){
    u64 *wait_semaphore_handles = SX_STACK_ARRAY_ALLOC(u64, wait_semaphores.Size());
    u64 *signal_semaphore_handles = SX_STACK_ARRAY_ALLOC(u64, signal_semaphores.Size());

    for(size_t i = 0; i<wait_semaphores.Size(); i++)
        wait_semaphore_handles[i] = wait_semaphores[i].Handle();

    for(size_t i = 0; i<signal_semaphores.Size(); i++)
        signal_semaphore_handles[i] = signal_semaphores[i].Handle();

    Execute(buffer, Span<u64>{wait_semaphore_handles, wait_semaphores.Size()}, Span<u64>{signal_semaphore_handles, signal_semaphores.Size()}, signal_fence);
}

void GPU::Execute(CommandBuffer *buffer, const Fence &signal_fence){
    Execute(buffer, Span<u64>(), Span<u64>(), signal_fence);
}

void GPU::Execute(CommandBuffer *buffer, const Semaphore &wait_semaphore, const Semaphore &signal_semaphore, const Fence &signal_fence){
    u64 wait_semaphore_handle = wait_semaphore.Handle();
    u64 signal_semaphore_handle = signal_semaphore.Handle();

    Execute(buffer, Span<u64>{&wait_semaphore_handle, 1}, Span<u64>{&signal_semaphore_handle, 1}, signal_fence);
}