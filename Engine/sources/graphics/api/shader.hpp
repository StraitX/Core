#ifndef STRAITX_SHADER_HPP
#define STRAITX_SHADER_HPP

#include "platform/types.hpp"
#include "platform/result.hpp"
#include "platform/file.hpp"
#include "core/noncopyable.hpp"
#include "core/validable.hpp"
#include "core/array_ptr.hpp"
#include "graphics/api/logical_gpu.hpp"

namespace StraitX{

class GraphicsAPILoader;

class Shader: public NonCopyable, public Validable{
public:
    enum Type: u8{
        Vertex,
        Geometry,
        TessellationControl,
        TessellationEvaluation,
        Fragment,
        Compute
    };
    enum class Lang: u8{
        Unknown = 0,
        SPIRV,
        GLSL,    
    };

    struct VTable{
        using NewProc    = Shader *(*)(LogicalGPU &owner, Shader::Type type, Shader::Lang lang, const u8 *sources, u32 length);
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

    constexpr Shader(Shader::Type type, Shader::Lang lang):
        m_Type(type),
        m_Lang(lang)
    {}

    virtual ~Shader() = default;

    constexpr Shader::Type GetType(){
        return m_Type;
    }

    constexpr Shader::Lang GetLang(){
        return m_Lang;
    }

    sx_inline static Shader *New(Shader::Type type, Shader::Lang lang, const u8 *sources, u32 length){
        return s_VTable.New(LogicalGPU::Instance(), type, lang, sources, length);
    }

    sx_inline static void Delete(Shader *shader){
        s_VTable.Delete(shader);
    }

};

}//namespace StraitX::

#endif //STRAITX_SHADER_HPP