#include <new>
#include "platform/memory.hpp"
#include "platform/opengl.hpp"
#include "core/log.hpp"
#include "graphics/opengl/shader_impl.hpp"

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
    
ShaderImpl::ShaderImpl(LogicalGPU& owner, Type type, Lang lang, const u8* sources, u32 length) :
    Shader(type, lang)
{
    //OpenGL, GPUless, we know...
    (void)owner;

    if (lang != Shader::Lang::GLSL)return;

    Handle = glCreateShader(GetStage(type));
    glShaderSource(Handle, 1, (const GLchar *const *)&sources, (s32*)&length);

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

Shader *ShaderImpl::NewImpl(LogicalGPU &owner, Type type, Lang lang, const u8 *sources, u32 length){
    return new (Memory::Alloc(sizeof(ShaderImpl))) ShaderImpl(owner, type, lang, sources, length);
}

void ShaderImpl::DeleteImpl(Shader *shader){
    shader->~Shader();
    Memory::Free(shader);
}

}//namespace GL::
}//namespace StraitX::