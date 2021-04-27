#ifndef STRAITX_OPENGL_GRAPHICS_API_IMPL_HPP
#define STRAITX_OPENGL_GRAPHICS_API_IMPL_HPP

#include "platform/opengl_context.hpp"
#include "graphics/api/graphics_api.hpp"
#include "graphics/api/gpu_configuration.hpp"

namespace StraitX{
namespace GL{

struct GraphicsAPIImpl: public GraphicsAPI{
private:
    OpenGLContext m_Context;

    Version m_LoadedOpenGLVersion = {};
    const char *m_VendorString    = nullptr;
    const char *m_RendererString  = nullptr;
    const char *m_VersionString   = nullptr;
    GPUVendor m_Vendor;

    s32 MaxTextureUnits;
    s32 MaxUniformBufferBindings;
public:
    static GraphicsAPIImpl Instance;

    virtual Result Initialize()override;

    virtual void Finalize()override;
};

}// namespace GL::
}// namespace StraitX::

#endif//STRAITX_OPENGL_GRAPHICS_API_IMPL_HPP