#ifndef STRAITX_VULKAN_SHADER_IMPL_HPP
#define STRAITX_VULKAN_SHADER_IMPL_HPP

#include "platform/vulkan.hpp"
#include "graphics/api/shader.hpp"
#include "graphics/vulkan/logical_gpu_impl.hpp"

namespace StraitX{
namespace Vk{

struct ShaderImpl: public Shader{
    Vk::LogicalGPUImpl &Owner;
    VkShaderModule Handle = VK_NULL_HANDLE;
    VkResult Status = VK_ERROR_INITIALIZATION_FAILED;

    ShaderImpl(LogicalGPU &owner, Type type, Lang lang, const u8 *sources, u32 length);

    ~ShaderImpl();

    bool IsValid()override;

    static VkShaderStageFlagBits GetStage(Shader::Type type);

    static Shader *NewImpl(LogicalGPU &owner, Type type, Lang lang, const u8 *sources, u32 length);

    static void DeleteImpl(Shader *shader);
};

}//namespace Vk::
}//namespace StraitX::

#endif//STRAITX_VULKAN_SHADER_IMPL_HPP