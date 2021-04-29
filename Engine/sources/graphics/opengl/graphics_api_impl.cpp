#include <new>
#include "platform/memory.hpp"
#include "platform/glloader.hpp"
#include "platform/opengl.hpp"
#include "core/log.hpp"
#include "core/string.hpp"
#include "servers/display_server.hpp"
#include "graphics/opengl/graphics_api_impl.hpp"

namespace StraitX{
namespace GL{

GraphicsAPIImpl GraphicsAPIImpl::Instance;

constexpr Version OpenGLVersion = {4, 6, 0};


GPUVendor ExtractGPUVendor(const char *string){
    auto length = String::Length(string);
    char *str = (char*)alloca(sizeof(char)*length);
    Memory::Copy(string, str, length);

    String::ToLowerCase(str);

    if(String::Contains(str, "nvidia"))
        return GPUVendor::Nvidia;
    if(String::Contains(str, "intel"))
        return GPUVendor::Intel;
    if(String::Contains(str, "amd"))
        return GPUVendor::AMD;
    if(String::Contains(str, "microsoft"))
        return GPUVendor::Microsoft;

    return GPUVendor::Unknown;
}

Result GraphicsAPIImpl::Initialize(){
    if (!m_Context.CreateDummy()) {
        LogError("OpenGLLoader: Can't create dummy OpenGL context");
        return Result::Unsupported;
    }
    if(!m_Context.MakeCurrent() || !OpenGLLoader::Load()){
        LogError("OpenGLLoader: Can't load OpenGL Procedures");
        m_Context.DestroyDummy();
        return Result::Unsupported;
    }
    m_Context.DestroyDummy();

    if(m_Context.Create(DisplayServer::Instance().GetWindow(), OpenGLVersion) != Result::Success)
        return Result::Failure;
    if(m_Context.MakeCurrent() != Result::Success)
        return Result::Unavailable;



    m_LoadedOpenGLVersion = OpenGLLoader::OpenGLVersion();
    m_VendorString = (const char*)glGetString(GL_VENDOR);
    m_RendererString = (const char *)glGetString(GL_RENDERER);
    m_VersionString = (const char*)glGetString(GL_VERSION);

    m_Vendor = ExtractGPUVendor(m_VendorString);
    LogInfo("OpenGL: Loaded Version %",m_LoadedOpenGLVersion);
    LogInfo("OpenGL: Renderer: %", m_RendererString);
    LogInfo("OpenGL: Vendor: %", m_VendorString);
    LogInfo("OpenGL: Version: %", m_VersionString);


    glEnable(GL_BLEND);

    glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &m_MaxTextureUnits);
    glGetIntegerv(GL_MAX_UNIFORM_BUFFER_BINDINGS, &m_MaxUniformBufferBindings);

    DLogInfo("GL: MaxTextureUnits:          %", m_MaxTextureUnits);
    DLogInfo("GL: MaxUniformBufferBindings: %", m_MaxUniformBufferBindings);

    return Result::Success;
}

void GraphicsAPIImpl::Finalize(){
    m_Context.Destroy();
}

}// namespace GL::
}// namespace StraitX::
