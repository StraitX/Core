#include "glsl2spv.hpp"
#include "core/assert.hpp"
#include "core/os/memory.hpp"
#include "graphics/vulkan/shader_impl.hpp"
#include "graphics/vulkan/gpu.hpp"

namespace Vk{

static glsl2spv::ShaderType GetGLSL2SPVShaderType(Shader::Type type){
    switch (type) {

    case Shader::Type::Vertex: return glsl2spv::ShaderType::Vertex;
    case Shader::Type::Geometry: return glsl2spv::ShaderType::Geometry;
    case Shader::Type::TessellationControl: return glsl2spv::ShaderType::TessellationControl;
    case Shader::Type::TessellationEvaluation: return glsl2spv::ShaderType::TessellationEvaluation;
    case Shader::Type::Fragment: return glsl2spv::ShaderType::Fragment;
    case Shader::Type::Compute: return glsl2spv::ShaderType::Compute;
    }
    SX_ASSERT(false);
    return glsl2spv::ShaderType(0);
}

ShaderImpl::ShaderImpl(Type type, Lang lang, const u8 *sources, u32 length):
    Shader(type, lang)
{
    if(!sources || !length)return;
    if(lang != Shader::Lang::SPIRV && lang != Shader::Lang::GLSL)return;

    std::vector<std::uint32_t> binary;
    if(lang == Shader::Lang::GLSL){
        if(!glsl2spv::CompileGLSL2SPV((const char *)sources, length, GetGLSL2SPVShaderType(type), binary))return;

        sources = (u8*)binary.data();
        length = binary.size() * sizeof(u32);
    }

    VkShaderModuleCreateInfo info;
    info.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    info.pNext = nullptr;
    info.flags = 0;
    info.codeSize = length;
    info.pCode = reinterpret_cast<const u32*>(sources);

    Status = vkCreateShaderModule(GPU::Get().Handle(), &info, nullptr, &Handle);
}

ShaderImpl::~ShaderImpl(){
    if(IsValid())
        vkDestroyShaderModule(GPU::Get().Handle(), Handle, nullptr);
}

bool ShaderImpl::IsValid()const{
    return Status == VK_SUCCESS;
}

VkShaderStageFlagBits ShaderImpl::GetStage(Shader::Type type) {
    switch (type)
    {
    case Shader::Vertex: return VK_SHADER_STAGE_VERTEX_BIT;
    case Shader::Geometry: return VK_SHADER_STAGE_GEOMETRY_BIT;
    case Shader::TessellationControl: return VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT;
    case Shader::TessellationEvaluation: return VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT;
    case Shader::Fragment: return VK_SHADER_STAGE_FRAGMENT_BIT;
    case Shader::Compute: return VK_SHADER_STAGE_COMPUTE_BIT;
    }
    return (VkShaderStageFlagBits)0;
}

Shader *ShaderImpl::NewImpl(Type type, Lang lang, const u8 *sources, u32 length){
    return new ShaderImpl(type, lang, sources, length);
}

void ShaderImpl::DeleteImpl(Shader *shader){
    delete shader;
}

}//namespace Vk::