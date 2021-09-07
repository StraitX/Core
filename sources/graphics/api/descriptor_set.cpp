#include "graphics/api/descriptor_set.hpp"
#include "graphics/api/graphics_api.hpp"
#include "core/os/vulkan.hpp"

#ifdef SX_VULKAN_SUPPORTED
    #include "graphics/api/vulkan/descriptor_set_impl.hpp"
#endif

DescriptorSetLayout *DescriptorSetLayout::Create(ConstSpan<ShaderBinding> bindings){
#ifdef SX_VULKAN_SUPPORTED
    if(GraphicsAPI::Backend() == GraphicsAPIBackend::Vulkan)
        return new Vk::DescriptorSetLayoutImpl(bindings);
#endif
    return nullptr;
}

DescriptorSetPool *DescriptorSetPool::Create(const DescriptorSetPoolProperties &props){
#ifdef SX_VULKAN_SUPPORTED
    if(GraphicsAPI::Backend() == GraphicsAPIBackend::Vulkan)
        return new Vk::DescriptorSetPoolImpl(props);
#endif

    return nullptr;
}