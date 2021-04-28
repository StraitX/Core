#ifndef STRAITX_VULKAN_GRAPHICS_API_IMPL_HPP
#define STRAITX_VULKAN_GRAPHICS_API_IMPL_HPP

#include "platform/vulkan.hpp"
#include "platform/result.hpp"
#include "platform/types.hpp"
#include "core/array_ptr.hpp"
#include "graphics/api/graphics_api.hpp"

namespace StraitX{
namespace Vk{

class GraphicsAPIImpl: public GraphicsAPI{
private:
    VkInstance m_Handle = VK_NULL_HANDLE;
    VkDebugUtilsMessengerEXT m_Messenger = VK_NULL_HANDLE;
public:
    static GraphicsAPIImpl Instance;

    virtual Result Initialize()override;

    virtual void Finalize()override;
    
    VkInstance Handle()const;
private:

    VkPhysicalDevice PickBestPhysicalDevice()const;

    static bool CheckLayers(const ArrayPtr<const char *> &layers);

    static bool CheckExtensions(const ArrayPtr<const char *> &extensions);
};

sx_inline VkInstance GraphicsAPIImpl::Handle()const{
    return m_Handle;
}

};//namespace Vk::
};//namespace StraitX::
#endif // STRAITX_VULKAN_GRAPHICS_API_IMPL_HPP