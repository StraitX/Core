#ifndef STRAITX_SHADER_HPP
#define STRAITX_SHADER_HPP

#include "core/types.hpp"
#include "core/span.hpp"
#include "graphics/api/graphics_resource.hpp"

constexpr size_t MaxUniformBindings = 8;


using ShaderStage = u8;

namespace ShaderStageBits{
    enum Value: ShaderStage{
        Vertex                  = 0x01,
        Geometry                = 0x02,
        TessellationControl     = 0x04,
        TessellationEvaluation  = 0x08,
        Fragment                = 0x10,
        Compute                 = 0x20
    };
}//namespace ShaderStageBits::

enum class ShaderLang: u8{
    Unknown = 0,
    GLSL,    
    //SPIRV,
};
//after creation of Pipeline shaders can be immediately destroyed
class Shader: public GraphicsResource{
public:
    virtual ~Shader() = default;

    virtual ShaderLang Lang()const = 0;

    virtual ShaderStageBits::Value Stage()const = 0;

    static Shader *Create(ShaderLang lang, ShaderStageBits::Value stage, Span<const char> sources);
};

#endif//STRAITX_SHADER_HPP