#ifndef STRAITX_VULKAN_SHADER_IMPL_HPP
#define STRAITX_VULKAN_SHADER_IMPL_HPP

#include "platform/vulkan.hpp"
#include "graphics/api/shader.hpp"

namespace StraitX{
namespace Vk{

struct ShaderImpl: public Shader{
    VkShaderModule Handle = VK_NULL_HANDLE;
    VkResult Status = VK_ERROR_INITIALIZATION_FAILED;

    ShaderImpl(Type type, Lang lang, const u8 *sources, u32 length);

    ~ShaderImpl();

    bool IsValid()override;

    static VkShaderStageFlagBits GetStage(Shader::Type type);

    static Shader *NewImpl(Type type, Lang lang, const u8 *sources, u32 length);

    static void DeleteImpl(Shader *shader);
};

}//namespace Vk::
}//namespace StraitX::

#endif//STRAITX_VULKAN_SHADER_IMPL_HPP