#include <new>
#include "platform/glloader.hpp"
#include "platform/opengl.hpp"
#include "core/log.hpp"
#include "graphics/opengl/graphics_api_impl.hpp"

namespace StraitX{
namespace GL{

constexpr Version OpenGLVersion = {4, 6, 0};

Result GraphicsAPIImpl::Initialize(){

    if(Context.CreateDummy(OpenGLVersion) != Result::Success)
        return Result::Failure;
    if(Context.MakeCurrent() != Result::Success)
        return Result::Unavailable;


    if(OpenGLLoader::Load() != Result::Success){
        LogError("OpenGLLoader: Can't load OpenGL Procedures");
        return Result::Unsupported;
    }
    auto ver = OpenGLLoader::OpenGLVersion();
    LogInfo("OpenGL: Loaded Version %",ver);
    LogInfo("OpenGL: Renderer: %", (const char*)glGetString(GL_RENDERER));
    LogInfo("OpenGL: Vendor: %", (const char*)glGetString(GL_VENDOR));


    return Result::Success;
}

Result GraphicsAPIImpl::Finalize(){
    Context.DestroyDummy();

    return Result::Success;
}

u32 GraphicsAPIImpl::GetPhysicalGPUCount(){
    // OpenGL allows us to control only one GPU
    return 1;
}

Result GraphicsAPIImpl::GetPhysicalGPUs(PhysicalGPU *array){
    const_cast<u64&>(array->Handle.U64) = 0;
    const_cast<GPUType&>(array->Type) = GPUType::Unknown;
    const_cast<GPUVendor&>(array->Vendor) = GPUVendor::Unknown;
    const_cast<u32&>(array->QueueFamiliesCount) = 1;

    return Result::Success;
}

}// namespace GL::
}// namespace StraitX::
