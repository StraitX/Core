#include "core/log.hpp"
#include "graphics/api/gpu_buffer.hpp"
#include "graphics/api/graphics_api.hpp"
#include "graphics/vulkan/gpu_buffer_impl.hpp"
#include "graphics/opengl/gpu_buffer_impl.hpp"


namespace StraitX{

GPUBuffer::GPUBuffer(){
        //const_cast<VTable::CreateProc&>(VirtualTable.Create) = &Vk::GPUBufferImpl::Create;
        //const_cast<VTable::DestroyProc&>(VirtualTable.Destroy) = &Vk::GPUBufferImpl::Destroy;
        //const_cast<VTable::CreateProc&>(VirtualTable.Create) = &GL::GPUBufferImpl::Create;
        //const_cast<VTable::DestroyProc&>(VirtualTable.Destroy) = &GL::GPUBufferImpl::Destroy;

}

}//namespace StraitX::