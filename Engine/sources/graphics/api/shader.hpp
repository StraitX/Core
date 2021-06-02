#ifndef STRAITX_SHADER_HPP
#define STRAITX_SHADER_HPP

#include "platform/types.hpp"
#include "platform/result.hpp"
#include "platform/file.hpp"
#include "core/noncopyable.hpp"
#include "core/validable.hpp"
#include "core/array_ptr.hpp"

namespace StraitX{

class GraphicsAPILoader;

//took it from head
constexpr size_t MaxUniformBindings = 8;

class Shader: public NonCopyable, public Validable{
public:
    typedef u8 Types;
    enum Type: Types{
        Vertex                  = 0x01,
        Geometry                = 0x02,
        TessellationControl     = 0x04,
        TessellationEvaluation  = 0x08,
        Fragment                = 0x10,
        Compute                 = 0x20
    };
    enum class Lang: u8{
        Unknown = 0,
        SPIRV,
        GLSL,    
    };

    struct VTable{
        using NewProc    = Shader *(*)(Shader::Type type, Shader::Lang lang, const u8 *sources, u32 length);
        using DeleteProc = void (*)(Shader *shader);

        NewProc    New    = nullptr;
        DeleteProc Delete = nullptr;
    };
private:
    static VTable s_VTable;

    Type m_Type;
    Lang m_Lang;

    friend class GraphicsAPILoader;
public:

    Shader(Shader::Type type, Shader::Lang lang);

    virtual ~Shader() = default;

    Shader::Type GetType()const;

    Shader::Lang GetLang()const;

    static Shader *New(Shader::Type type, Shader::Lang lang, const u8 *sources, u32 length);

    static Shader *New(Shader::Type type, Shader::Lang lang, const char *filename);

    static void Delete(Shader *shader);

};

SX_INLINE Shader::Shader(Shader::Type type, Shader::Lang lang):
    m_Type(type),
    m_Lang(lang)
{}

SX_INLINE Shader::Type Shader::GetType()const{
    return m_Type;
}

SX_INLINE Shader::Lang Shader::GetLang()const{
    return m_Lang;
}

SX_INLINE Shader *Shader::New(Shader::Type type, Shader::Lang lang, const u8 *sources, u32 length){
    return s_VTable.New(type, lang, sources, length);
}

SX_INLINE void Shader::Delete(Shader *shader){
    s_VTable.Delete(shader);
}

}//namespace StraitX::

#endif //STRAITX_SHADER_HPP