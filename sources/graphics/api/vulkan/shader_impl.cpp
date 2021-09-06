#include "graphics/api/vulkan/shader_impl.hpp"
#include "graphics/api/vulkan/gpu_impl.hpp"
#include "glsl2spv.hpp"
#include "core/log.hpp"

namespace Vk{


glsl2spv::ShaderType ToShaderType(ShaderStageBits::Value stage){
    switch (stage) {
    case ShaderStageBits::Vertex: 
        return glsl2spv::ShaderType::Vertex;
    case ShaderStageBits::Geometry: 
        return glsl2spv::ShaderType::Geometry;
    case ShaderStageBits::TessellationControl: 
        return glsl2spv::ShaderType::TessellationControl;
    case ShaderStageBits::TessellationEvaluation: 
        return glsl2spv::ShaderType::TessellationEvaluation;
    case ShaderStageBits::Fragment: 
        return glsl2spv::ShaderType::Fragment;
    case ShaderStageBits::Compute: 
        return glsl2spv::ShaderType::Compute;
    }
}


ShaderImpl::ShaderImpl(ShaderLang lang, ShaderStageBits::Value stage, Span<const char> sources):
    m_Lang(lang),
    m_Stage(stage)
{

    std::vector<std::uint32_t> spv_binary;

    if(lang == ShaderLang::GLSL){
        // XXX Validation
        if(!glsl2spv::CompileGLSL2SPV(sources.Pointer(), sources.Size(), ToShaderType(stage), spv_binary))
            LogError("Vk: ShaderImpl: failed to compile GLSL to SPIR-V");

        sources = {(const char*)spv_binary.data(), spv_binary.size() * sizeof(std::uint32_t)};
    }


    VkShaderModuleCreateInfo info;
    info.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    info.pNext = nullptr;
    info.flags = 0;
    info.codeSize = sources.Size();
    info.pCode = (u32*)sources.Pointer();

    vkCreateShaderModule(GPUImpl::s_Instance, &info, nullptr, &m_Handle);
}

ShaderImpl::~ShaderImpl(){
    vkDestroyShaderModule(GPUImpl::s_Instance, m_Handle, nullptr);
}

ShaderLang ShaderImpl::Lang()const{
    return m_Lang;
}

ShaderStageBits::Value ShaderImpl::Stage()const{
    return m_Stage;
}

}//namespace Vk::