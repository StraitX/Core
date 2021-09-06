#include "graphics/api/shader.hpp"
#include "graphics/api/graphics_api.hpp"
#include "core/os/vulkan.hpp"

#ifdef SX_VULKAN_SUPPORTED
    #include "graphics/api/vulkan/shader_impl.hpp"
#endif

Shader *Shader::Create(ShaderLang lang, ShaderStageBits::Value stage, Span<const char> sources){
#ifdef SX_VULKAN_SUPPORTED
    if(GraphicsAPI::Backend() == GraphicsAPIBackend::Vulkan)
        return new Vk::ShaderImpl(lang, stage, sources);
#endif
    return nullptr;
}