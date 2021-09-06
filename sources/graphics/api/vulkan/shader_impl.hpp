#ifndef STRAITX_VULKAN_SHADER_IMPL_HPP
#define STRAITX_VULKAN_SHADER_IMPL_HPP

#include "core/os/vulkan.hpp"
#include "graphics/api/shader.hpp"

namespace Vk{

class ShaderImpl: public Shader{
private:
    VkShaderModule m_Handle = VK_NULL_HANDLE;
    ShaderLang m_Lang;
    ShaderStageBits::Value m_Stage;
public:
    ShaderImpl(ShaderLang lang, ShaderStageBits::Value stage, Span<const char> sources);

    ~ShaderImpl();

    ShaderLang Lang()const override;

    ShaderStageBits::Value Stage()const override;
};

}//namespace Vk::

#endif//STRAITX_VULKAN_SHADER_IMPL_HPP