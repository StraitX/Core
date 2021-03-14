#ifndef STRAITX_OPENGL_SHADER_IMPL_HPP
#define STRAITX_OPENGL_SHADER_IMPL_HPP 

#include "core/push_array.hpp"
#include "graphics/api/shader.hpp"

namespace StraitX{
namespace GL{

struct ShaderImpl: public Shader{
    u32 Handle = 0;
    size_t SourcesLength = 0;
    PushArray<u32, MaxUniformBindings> UniformBindings;

    ShaderImpl(LogicalGPU &owner, Type type, Lang lang, const u8 *sources, u32 length);

    ~ShaderImpl();

    bool IsValid()override;

    void Compile();

    bool RequiresPreprocess()const;

    static Shader *NewImpl(LogicalGPU &owner, Type type, Lang lang, const u8 *sources, u32 length);

    static void DeleteImpl(Shader *shader);
};

}//namespace GL::
}//namespace StraitX::

#endif//STRAITX_OPENGL_SHADER_IMPL_HPP