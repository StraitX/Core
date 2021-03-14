#include <new>
#include <cstdio>
#include "platform/memory.hpp"
#include "platform/opengl.hpp"
#include "platform/compiler.hpp"
#include "core/log.hpp"
#include "core/string.hpp"
#include "graphics/opengl/shader_impl.hpp"
#include "graphics/opengl/graphics_api_impl.hpp"

namespace StraitX{
namespace GL{

static GLenum GetStage(Shader::Type type) {
    switch (type)
    {
    case StraitX::Shader::Vertex: return GL_VERTEX_SHADER;
    case StraitX::Shader::Geometry: return GL_GEOMETRY_SHADER;
    case StraitX::Shader::TessellationControl: return GL_TESS_CONTROL_SHADER;
    case StraitX::Shader::TessellationEvaluation: return GL_TESS_EVALUATION_SHADER;
    case StraitX::Shader::Fragment: return GL_FRAGMENT_SHADER;
    case StraitX::Shader::Compute: return GL_COMPUTE_SHADER;
    }
    return 0;
}
    
ShaderImpl::ShaderImpl(LogicalGPU& owner, Type type, Lang lang, const u8* src, u32 length) :
    Shader(type, lang),
    SourcesLength(length)
{
    //OpenGL, GPUless, we know...
    (void)owner;

    if (lang != Shader::Lang::GLSL)return;

#ifdef SX_DEBUG
    {
        char *input = (char *)alloca(length + 1);
        Memory::Copy((const char *)src, input, length);
        input[length] = 0;

        for(size_t i = 0; i<length;){
            size_t len = String::LineLength(input);
            if(String::Find(input, len, "uniform")){
                CoreAssert(String::Find(input, len, "binding")!=nullptr, "GL: GLSL: uniform should have an explicit binding");
                CoreAssert(String::Find(input, len, "std140")!=nullptr, "GL: GLSL: uniform should have std140 layout specified");
            }
            i+=len;
            input+=len;
        }
        
    }
#endif

    Handle = glCreateShader(GetStage(type));

    if(RequiresPreprocess())
    {
        LogWarn("OpenGL: Shader requires translation for backward compatability");

        char *input = (char *)alloca(length + 1);
        Memory::Copy((const char *)src, input, length);
        input[length] = 0;
        size_t i = String::LineLength(input);

        char *output = (char *)alloca(length + 1);
        size_t output_written = 0;

        constexpr const char *header = "#version 410 core\n";
        size_t header_len = String::LineLength(header);
        Memory::Copy(header, output, header_len);
        output_written += header_len;

        const char *binding = nullptr;

        while((binding = String::Find(input + i,"binding"))){
            for(;(input+i) != binding;++i)
                output[output_written++]=*(input + i);
            while(output[output_written]!=',')
                --output_written;
            
            i += 7;

            while(*(input + i) == ' ' || *(input + i) == '=')
                ++i;
            
            u32 index = -1;
            std::sscanf((input + i), "%u",&index);
            UniformBindings.Push(index);

            while(*(input + i) >= '0' && *(input + i) <= '9')
                ++i;
        }
        for(; i<length + 1; ++i)
            output[output_written++] = input[i];
        LogInfo("Bindings: %",UniformBindings.Size());

        glShaderSource(Handle, 1, (const GLchar *const *)&output, (s32*)&length);
    }else{
        glShaderSource(Handle, 1, (const GLchar *const *)&src, (s32*)&length);
    }
    Compile();
}

ShaderImpl::~ShaderImpl(){
    if(Handle)
        glDeleteShader(Handle);
}

bool ShaderImpl::IsValid(){
    if(Handle == 0)return false;

    int status;
    glGetShaderiv(Handle, GL_COMPILE_STATUS, &status);
    
    return status == GL_TRUE;
}

void ShaderImpl::Compile(){
    glCompileShader(Handle);

    if(IsValid())return;

    int log_length;
    glGetShaderiv(Handle, GL_INFO_LOG_LENGTH, &log_length);

    char *log = (char *)alloca((log_length + 1) * sizeof(char));
    log[log_length] = 0;
    glGetShaderInfoLog(Handle, log_length, &log_length, log);

    LogError("GL: Shader Compilation failed: %",log);
}

bool ShaderImpl::RequiresPreprocess()const{
    auto &version = GraphicsAPIImpl::Instance.LoadedOpenGLVersion;
    return !(version.Major >= 4 && version.Minor >= 2);
}

Shader *ShaderImpl::NewImpl(LogicalGPU &owner, Type type, Lang lang, const u8 *sources, u32 length){
    return new (Memory::Alloc(sizeof(ShaderImpl))) ShaderImpl(owner, type, lang, sources, length);
}

void ShaderImpl::DeleteImpl(Shader *shader){
    shader->~Shader();
    Memory::Free(shader);
}

}//namespace GL::
}//namespace StraitX::