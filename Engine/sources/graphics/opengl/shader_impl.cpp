#include <new>
#include "platform/memory.hpp"
#include "platform/opengl.hpp"
#include "core/log.hpp"
#include "graphics/opengl/shader_impl.hpp"

namespace StraitX{
namespace GL{

GLenum ShaderTypesTable[]={
    GL_VERTEX_SHADER,
    GL_GEOMETRY_SHADER,
    GL_TESS_CONTROL_SHADER,
    GL_TESS_EVALUATION_SHADER,
    GL_FRAGMENT_SHADER,
    GL_COMPUTE_SHADER
};

ShaderImpl::ShaderImpl(LogicalGPU &owner, Type type, Lang lang, const u8 *sources, u32 length){
    //OpenGL, GPUless, we know...
    (void)owner;

    if(lang != Shader::Lang::GLSL)return;

    Handle = glCreateShader(ShaderTypesTable[type]);
    glShaderSource(Handle, 1, (const GLchar *const *)&sources, (i32*)&length);

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