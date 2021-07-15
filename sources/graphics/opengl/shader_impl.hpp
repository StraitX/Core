#ifndef STRAITX_OPENGL_SHADER_IMPL_HPP
#define STRAITX_OPENGL_SHADER_IMPL_HPP 

#include "platform/opengl.hpp"
#include "core/push_array.hpp"
#include "graphics/api/shader.hpp"

namespace GL{

struct ShaderImpl: public Shader{
    char *Sources = nullptr;
    u32 Length = 0;
    Type ShaderType;
    Lang ShaderLang;

    ShaderImpl(Type type, Lang lang, const u8 *sources, u32 length);

    ~ShaderImpl();

    bool IsValid()const override;

    static GLenum GetStage(Type type);

    static Shader *NewImpl(Type type, Lang lang, const u8 *sources, u32 length);

    static void DeleteImpl(Shader *shader);

};

}//namespace GL::

#endif//STRAITX_OPENGL_SHADER_IMPL_HPP