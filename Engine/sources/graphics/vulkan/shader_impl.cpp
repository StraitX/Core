#include "core/assert.hpp"
#include "graphics/vulkan/shader_impl.hpp"

namespace StraitX{
namespace Vk{

    
sx_inline ShaderImpl::ShaderImpl(LogicalGPU &owner, Type type, Lang lang, const u8 *sources, u32 length):
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

sx_inline ShaderImpl::~ShaderImpl(){
    vkDestroyShaderModule(Owner.Handle, Handle, nullptr);
}

bool ShaderImpl::IsValid(){
    return Status == VK_SUCCESS;
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