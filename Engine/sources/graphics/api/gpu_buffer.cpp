#include "core/log.hpp"
#include "graphics/api/gpu_buffer.hpp"
#include "graphics/api/graphics_api.hpp"
#include "graphics/opengl/gpu_buffer_impl.hpp"


namespace StraitX{

GPUBuffer::GPUBuffer(){
    switch (GraphicsAPI::Instance().CurrentAPI()) {
    case GraphicsAPI::OpenGL:
        VirtualTable.Create = &GL::GPUBufferImpl::Create;
        VirtualTable.Destroy = &GL::GPUBufferImpl::Destroy;
        return;
    default:
        LogWarn("GPUBuffer: is not supported");
    }
}

}//namespace StraitX::