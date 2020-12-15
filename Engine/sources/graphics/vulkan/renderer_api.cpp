#include "graphics/vulkan/renderer_api.hpp"
#include <new>
namespace StraitX{
namespace Vk{

RendererAPI RendererAPI::Instance;

Error RendererAPI::Initialize(){
    constexpr Version vk_version = {1,0,0};
    const char *extensions[]={
        
    };
    const char *layers[]={

    };
    m_Instance.Create(vk_version, extensions, sizeof(extensions)/sizeof(char*), layers, sizeof(layers)/sizeof(char*));

    ArrayPtr<VkPhysicalDevice,u32> devices;
    vkEnumeratePhysicalDevices(m_Instance.Handle,&devices.Size,nullptr);

    m_PhysicalDeivces.Size = devices.Size;
    m_PhysicalDeivces.Pointer = (Vk::PhysicalDevice *)m_Allocator.Alloc(sizeof(Vk::PhysicalDevice)*m_PhysicalDeivces.Size);

    // allocate memory for physical deices to free them in StackAllocator fashion
    devices.Pointer = (VkPhysicalDevice *)m_Allocator.Alloc(devices.Size * sizeof(VkPhysicalDevice));
    vkEnumeratePhysicalDevices(m_Instance.Handle,&devices.Size,devices.Pointer);

    for(int i = 0; i<devices.Size; i++){
        new (m_PhysicalDeivces.Pointer + i) Vk::PhysicalDevice(devices[i]);
    }
    // here we go
    m_Allocator.Free(devices.Size*sizeof(VkPhysicalDevice));
    
    


    return Error::Success;
}


Error RendererAPI::Finalize(){
    return Error::Success;
}


};//namespace Vk::
};//namespace StraitX::