#include <new>
#include "platform/glloader.hpp"
#include "platform/opengl.hpp"
#include "core/log.hpp"
#include "core/string.hpp"
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
    LoadedOpenGLVersion = OpenGLLoader::OpenGLVersion();
    LogInfo("OpenGL: Loaded Version %",LoadedOpenGLVersion);
    LogInfo("OpenGL: Renderer: %", (const char*)glGetString(GL_RENDERER));
    LogInfo("OpenGL: Vendor: %", (const char*)glGetString(GL_VENDOR));
    LogInfo("OpenGL: Version: %", (const char*)glGetString(GL_VERSION));


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


GPUVendor ExtractGPUVendor(const char *string){
    auto length = String::Length(string);
    char *str = (char*)alloca(sizeof(char)*length);
    Memory::Copy(string, str, length);

    String::ToLowerCase(str);

    if(String::Contain(str, "nvidia"))
        return GPUVendor::Nvidia;
    if(String::Contain(str, "intel"))
        return GPUVendor::Intel;
    if(String::Contain(str, "amd"))
        return GPUVendor::AMD;
    if(String::Contain(str, "microsoft"))
        return GPUVendor::Microsoft;

    return GPUVendor::Unknown;
}

static_assert(sizeof(Version) == sizeof(u64), "sizeof(Version) should match sizeof(u64) to be inserted as Handle to OpenGL Physical GPU");

Result GraphicsAPIImpl::GetPhysicalGPUs(PhysicalGPU *array){

    const_cast<u64&>(array->Handle.U64) = *(u64*)&LoadedOpenGLVersion;
    const_cast<GPUType&>(array->Type) = GPUType::Unknown;
    const_cast<GPUVendor&>(array->Vendor) = ExtractGPUVendor((const char *)glGetString(GL_VENDOR));
    const_cast<u32&>(array->QueueFamiliesCount) = 1;

    return Result::Success;
}

}// namespace GL::
}// namespace StraitX::
