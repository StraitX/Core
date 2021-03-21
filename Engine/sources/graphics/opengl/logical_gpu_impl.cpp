#include "platform/opengl.hpp"
#include "core/assert.hpp"
#include "core/log.hpp"
#include "graphics/opengl/logical_gpu_impl.hpp"
#include "graphics/opengl/cpu_buffer_impl.hpp"
#include "servers/display_server.hpp"

namespace StraitX{
namespace GL{

LogicalGPUImpl LogicalGPUImpl::Instance;

Result LogicalGPUImpl::Initialize(const PhysicalGPU &gpu){
    if(Context.Create(DisplayServer::Instance().GetWindow(), *(Version*)&gpu.Handle.U64) != Result::Success)
        return Result::Unsupported;
    if(Context.MakeCurrent() != Result::Success)
        return Result::Unavailable;
        
    glEnable(GL_BLEND);

    glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &MaxTextureUnits);
    glGetIntegerv(GL_MAX_UNIFORM_BUFFER_BINDINGS, &MaxUniformBufferBindings);

    DLogInfo("GL: MaxTextureUnits:          %", MaxTextureUnits);
    DLogInfo("GL: MaxUniformBufferBindings: %", MaxUniformBufferBindings);

    return Result::Success;
}

void LogicalGPUImpl::Finalize(){
    Context.Destroy();
}

void LogicalGPUImpl::SwapBuffers(){
    Context.SwapBuffers();
}


}//namespace GL::
}//namespace StraitX::