#ifndef STRAITX_VULKAN_GRAPHICS_API_IMPL_HPP
#define STRAITX_VULKAN_GRAPHICS_API_IMPL_HPP

#include "graphics/api/graphics_api.hpp"
#include "core/os/vulkan.hpp"

namespace Vk{

class GraphicsAPIBackendImpl: public ::GraphicsAPIBackendImpl{
private:
    VkInstance m_Instance = VK_NULL_HANDLE;
    VkDebugUtilsMessengerEXT m_Messenger = VK_NULL_HANDLE;

public:
    static GraphicsAPIBackendImpl s_Instance;

    Result Create()override;

    void Destroy()override;
};

}//namespace Vk::

#endif//STRAITX_VULKAN_GRAPHICS_API_IMPL_HPP