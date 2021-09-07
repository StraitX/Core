#include "graphics/api/sampler.hpp"
#include "graphics/api/graphics_api.hpp"
#include "core/os/vulkan.hpp"

#ifdef SX_VULKAN_SUPPORTED
    #include "graphics/api/vulkan/sampler_impl.hpp"
#endif

Sampler *Sampler::Create(const SamplerProperties &props){
#ifdef SX_VULKAN_SUPPORTED
    if(GraphicsAPI::Backend() == GraphicsAPIBackend::Vulkan)
        return new Vk::SamplerImpl(props);
#endif

    return nullptr;
}
