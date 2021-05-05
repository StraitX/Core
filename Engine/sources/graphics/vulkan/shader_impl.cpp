#include "glsl2spv.hpp"
#include "core/assert.hpp"
#include "platform/memory.hpp"
#include "graphics/vulkan/shader_impl.hpp"
#include "graphics/vulkan/gpu.hpp"

namespace StraitX{
namespace Vk{

static glsl2spv::ShaderType GetGLSL2SPVShaderType(Shader::Type type){
    switch (type) {

    case Shader::Type::Vertex: return glsl2spv::ShaderType::Vertex;
    case Shader::Type::Geometry: return glsl2spv::ShaderType::Geometry;
    case Shader::Type::TessellationControl: return glsl2spv::ShaderType::TesselationControl;
    case Shader::Type::TessellationEvaluation: return glsl2spv::ShaderType::TesselationEvaluation;
    case Shader::Type::Fragment: return glsl2spv::ShaderType::Fragment;
    case Shader::Type::Compute: return glsl2spv::ShaderType::Compute;
    }
    Assert(false);
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
    case StraitX::Shader::Vertex: return VK_SHADER_STAGE_VERTEX_BIT;
    case StraitX::Shader::Geometry: return VK_SHADER_STAGE_GEOMETRY_BIT;
    case StraitX::Shader::TessellationControl: return VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT;
    case StraitX::Shader::TessellationEvaluation: return VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT;
    case StraitX::Shader::Fragment: return VK_SHADER_STAGE_FRAGMENT_BIT;
    case StraitX::Shader::Compute: return VK_SHADER_STAGE_COMPUTE_BIT;
    }
    return (VkShaderStageFlagBits)0;
}

Shader *ShaderImpl::NewImpl(Type type, Lang lang, const u8 *sources, u32 length){
    return new(Memory::Alloc(sizeof(Vk::ShaderImpl))) ShaderImpl(type, lang, sources, length);
}

void ShaderImpl::DeleteImpl(Shader *shader){
    shader->~Shader();
    Memory::Free(shader);
}

}//namespace Vk::
}//namespace StraitX::