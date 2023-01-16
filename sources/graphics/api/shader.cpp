#include "graphics/api/shader.hpp"
#include "graphics/api/graphics_api.hpp"
#include "core/os/vulkan.hpp"

#ifdef SX_VULKAN_SUPPORTED
    #include "graphics/api/vulkan/shader_impl.hpp"
#endif

Shader *Shader::Create(ShaderStageBits::Value stage, StringView sources, ShaderCompileOptions opts){
#ifdef SX_VULKAN_SUPPORTED
    if(GraphicsAPI::Backend() == GraphicsAPIBackend::Vulkan)
        return new Vk::ShaderImpl(stage, sources, opts);
#endif
    return nullptr;
}