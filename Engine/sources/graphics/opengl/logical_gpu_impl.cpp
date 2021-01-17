#include "graphics/opengl/logical_gpu_impl.hpp"
#include "servers/display_server.hpp"

namespace StraitX{
namespace GL{

Result LogicalGPUImpl::Create(const PhysicalGPU &gpu){
    if(m_Context.Create(DisplayServer::Instance().GetWindow(), *(Version*)&gpu.Handle.U64) != Result::Success)
        return Result::Failure;
    if(m_Context.MakeCurrent() != Result::Success)
        return Result::Unavailable;

    return Result::Success;
}

void LogicalGPUImpl::Destroy(){
    m_Context.Destroy();
}


}//namespace GL::
}//namespace StraitX::