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
    
ShaderImpl::ShaderImpl(Type type, Lang lang, const u8* src, u32 length) :
    Shader(type, lang),
    Length(length),
    ShaderType(type),
    ShaderLang(lang)
{
    if(!src || !Length)return;

    // XXX: i assume that shader source code is totally valid now
    Sources = (char *)Memory::Alloc(Length + 1);
    Memory::Copy(src, Sources, Length);
    Sources[Length] = 0;
}

ShaderImpl::~ShaderImpl(){
    Memory::Free(Sources);
}

bool ShaderImpl::IsValid()const{
    return ShaderLang == Shader::Lang::GLSL && Sources && Length;
}

GLenum ShaderImpl::GetStage(Type type) {
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

Shader *ShaderImpl::NewImpl(Type type, Lang lang, const u8 *sources, u32 length){
    return new (Memory::Alloc(sizeof(ShaderImpl))) ShaderImpl(type, lang, sources, length);
}

void ShaderImpl::DeleteImpl(Shader *shader){
    shader->~Shader();
    Memory::Free(shader);
}

}//namespace GL::
}//namespace StraitX::