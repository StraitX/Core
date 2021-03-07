#include "core/assert.hpp"
#include "platform/memory.hpp"
#include "graphics/vulkan/shader_impl.hpp"

namespace StraitX{
namespace Vk{

ShaderImpl::ShaderImpl(LogicalGPU &owner, Type type, Lang lang, const u8 *sources, u32 length):
    Shader(type, lang),
    Owner(static_cast<Vk::LogicalGPUImpl&>(owner))
{
    if(lang != Shader::Lang::SPIRV)return;
    
    VkShaderModuleCreateInfo info;
    info.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    info.pNext = nullptr;
    info.flags = 0;
    info.codeSize = length;
    info.pCode = reinterpret_cast<const u32*>(sources);

    Status = vkCreateShaderModule(Owner.Handle, &info, nullptr, &Handle);
}

ShaderImpl::~ShaderImpl(){
    if(IsValid())
        vkDestroyShaderModule(Owner.Handle, Handle, nullptr);
}

bool ShaderImpl::IsValid(){
    return Status == VK_SUCCESS;
}

VkShaderStageFlagBits ShaderImpl::GetStage(Shader::Type type) {
    switch (type)
    {
    case StraitX::Shader::Vertex: return VK_SHADER_STAGE_VERTEX_BIT;
    case StraitX::Shader::Geometry: return VK_SHADER_STAGE_GEOMETRY_BIT;
    case StraitX::Shader::TessellationControl: return VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT;
    case StraitX::Shader::TessellationEvaluation: return VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT;
    case StraitX::Shader::Fragment: return VK_SHADER_STAGE_FRAGMENT_BIT;
    case StraitX::Shader::Compute: return VK_SHADER_STAGE_COMPUTE_BIT;
    }
    return (VkShaderStageFlagBits)0;
}

Shader *ShaderImpl::NewImpl(LogicalGPU &owner, Type type, Lang lang, const u8 *sources, u32 length){
    return new(Memory::Alloc(sizeof(Vk::ShaderImpl))) ShaderImpl(owner, type, lang, sources, length);
}

void ShaderImpl::DeleteImpl(Shader *shader){
    shader->~Shader();
    Memory::Free(shader);
}

}//namespace Vk::
}//namespace StraitX::