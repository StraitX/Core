#include "core/assert.hpp"
#include "graphics/opengl/logical_gpu_impl.hpp"
#include "servers/display_server.hpp"

namespace StraitX{
namespace GL{

LogicalGPUImpl::LogicalGPUImpl(const PhysicalGPU &gpu){
    CoreAssert(m_Context.Create(DisplayServer::Instance().GetWindow(), *(Version*)&gpu.Handle.U64) == Result::Success, "GL::LogicalGPUImpl: Can't createa OpenGL Context");
    CoreAssert(m_Context.MakeCurrent() == Result::Success, "GL::LogicalGPUImpl: Can't make OpenGL context current");
}

LogicalGPUImpl::~LogicalGPUImpl(){
    m_Context.Destroy();
}


}//namespace GL::
}//namespace StraitX::