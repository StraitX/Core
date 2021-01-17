#ifndef STRAITX_OPENGL_GRAPHICS_API_IMPL_HPP
#define STRAITX_OPENGL_GRAPHICS_API_IMPL_HPP

#include "platform/glcontext.hpp"
#include "graphics/api/graphics_api.hpp"


namespace StraitX{
namespace GL{

struct GraphicsAPIImpl: public GraphicsAPI{
    GLContext Context;
    Version LoadedOpenGLVersion = {};

    virtual Result Initialize()override;

    virtual Result Finalize()override;

    virtual u32 GetPhysicalGPUCount()override;

    virtual Result GetPhysicalGPUs(PhysicalGPU *array)override;
};

}// namespace GL::
}// namespace StraitX::

#endif//STRAITX_OPENGL_GRAPHICS_API_IMPL_HPP