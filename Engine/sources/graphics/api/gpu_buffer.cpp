#include "core/log.hpp"
#include "graphics/api/gpu_buffer.hpp"
#include "graphics/api/graphics_api.hpp"
#include "graphics/vulkan/gpu_buffer_impl.hpp"
#include "graphics/opengl/gpu_buffer_impl.hpp"


namespace StraitX{

GPUBuffer::GPUBuffer(){
    switch (GraphicsAPI::Instance().CurrentAPI()) {
    case GraphicsAPI::Vulkan:
        const_cast<VTable::CreateProc&>(VirtualTable.Create) = &Vk::GPUBufferImpl::Create;
        const_cast<VTable::DestroyProc&>(VirtualTable.Destroy) = &Vk::GPUBufferImpl::Destroy;
        return;
    case GraphicsAPI::OpenGL:
        const_cast<VTable::CreateProc&>(VirtualTable.Create) = &GL::GPUBufferImpl::Create;
        const_cast<VTable::DestroyProc&>(VirtualTable.Destroy) = &GL::GPUBufferImpl::Destroy;
        return;
    default:
        LogWarn("GPUBuffer: is not supported");
    }
}

}//namespace StraitX::