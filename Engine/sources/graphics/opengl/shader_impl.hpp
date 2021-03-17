#ifndef STRAITX_OPENGL_SHADER_IMPL_HPP
#define STRAITX_OPENGL_SHADER_IMPL_HPP 

#include "platform/opengl.hpp"
#include "core/push_array.hpp"
#include "graphics/api/shader.hpp"

namespace StraitX{
namespace GL{


struct ShaderImpl: public Shader{
    char *Sources;
    u32 Length;
    Type ShaderType;
    Lang ShaderLang;

    ShaderImpl(LogicalGPU &owner, Type type, Lang lang, const u8 *sources, u32 length);

    ~ShaderImpl();

    bool IsValid()override;

    static GLenum GetStage(Type type);

    static Shader *NewImpl(LogicalGPU &owner, Type type, Lang lang, const u8 *sources, u32 length);

    static void DeleteImpl(Shader *shader);

};

}//namespace GL::
}//namespace StraitX::

#endif//STRAITX_OPENGL_SHADER_IMPL_HPP