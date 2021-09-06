#include "graphics/api/render_pass.hpp"
#include "graphics/api/graphics_api.hpp"
#include "core/os/vulkan.hpp"

#ifdef SX_VULKAN_SUPPORTED
    #include "graphics/api/vulkan/render_pass_impl.hpp"
#endif

RenderPass *RenderPass::Create(const RenderPassProperties &props){
#ifdef SX_VULKAN_SUPPORTED
    if(GraphicsAPI::Backend() == GraphicsAPIBackend::Vulkan)
        return new Vk::RenderPassImpl(props);
#endif
    return nullptr;
}
