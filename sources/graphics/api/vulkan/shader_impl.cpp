#include "graphics/api/vulkan/shader_impl.hpp"
#include "graphics/api/vulkan/gpu_impl.hpp"
#include "glsl2spv.hpp"
#include "core/log.hpp"
#include "core/os/file.hpp"

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

VkShaderStageFlagBits ToVkShaderStage(ShaderStageBits::Value stage){
    return (VkShaderStageFlagBits)stage;
}

struct Includer : glsl2spv::Includer {
    ShaderCompileOptions m_Opts;

    Includer(ShaderCompileOptions opts):
        m_Opts(opts)
    {}

    std::string Include(const char* header_name)const override {
        if (File::Exists(header_name)) {
            auto content = File::ReadEntire(header_name).Value();
            return { content.Data(), content.Size() };
        }

        const auto NormalizePath = [](StringView path)->String {
            return path.Data()[path.Size() - 1] == '/' ? String(path) : String(path) + "/";
        };

        for (StringView dir : m_Opts.IncludeDirectories) {
            if (!dir.Size())
                continue;

            String full_path = NormalizePath(dir) + header_name;

            auto content = File::ReadEntire(full_path);
            if (content.HasValue())
                return { content.Value().Data(), content.Value().Size() };
        }

        return {};
    }
};

ShaderImpl::ShaderImpl(ShaderStageBits::Value stage, StringView sources, ShaderCompileOptions opts):
    m_Lang(opts.Lang),
    m_Stage(stage)
{

    std::vector<std::uint32_t> spv_binary;

    String processed_sources
        = String("#version 440 core\n#extension GL_GOOGLE_include_directive : require\n")
        + sources;

    if(opts.Lang == ShaderLang::GLSL){
        // XXX Validation

        Includer includer(opts);
        if (!glsl2spv::CompileGLSL2SPV(processed_sources.Data(), processed_sources.Size(), ToShaderType(stage), spv_binary, includer))
            LogError("Vk: ShaderImpl: failed to compile GLSL to SPIR-V");

        sources = {(const char*)spv_binary.data(), spv_binary.size() * sizeof(std::uint32_t)};
    }


    VkShaderModuleCreateInfo info;
    info.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    info.pNext = nullptr;
    info.flags = 0;
    info.codeSize = sources.Size();
    info.pCode = (u32*)sources.Data();

    vkCreateShaderModule(GPUImpl::Get(), &info, nullptr, &m_Handle);
}

ShaderImpl::~ShaderImpl(){
    vkDestroyShaderModule(GPUImpl::Get(), m_Handle, nullptr);
}

ShaderLang ShaderImpl::Lang()const{
    return m_Lang;
}

ShaderStageBits::Value ShaderImpl::Stage()const{
    return m_Stage;
}

}//namespace Vk::