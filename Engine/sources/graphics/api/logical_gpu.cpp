#include "core/log.hpp"
#include "graphics/api/logical_gpu.hpp"
#include "graphics/api/graphics_api.hpp"
#include "graphics/vulkan/logical_gpu_impl.hpp"
#include "graphics/opengl/logical_gpu_impl.hpp"

namespace StraitX{

Result LogicalGPU::Initialize(const PhysicalGPU &gpu){
    switch (GraphicsAPI::CurrentAPI()) {
    case GraphicsAPI::Vulkan:
        return Vk::LogicalGPUImpl::Instance.Initialize(gpu);
    case GraphicsAPI::OpenGL:
        return GL::LogicalGPUImpl::Instance.Initialize(gpu);
    default:
        LogWarn("LogicalGPU::Initialize: Is not supported by current API");
        return Result::NotFound;
    }
}

void LogicalGPU::Finalize(){
    switch (GraphicsAPI::CurrentAPI()) {
    case GraphicsAPI::Vulkan:
        Vk::LogicalGPUImpl::Instance.Finalize();
    case GraphicsAPI::OpenGL:
        GL::LogicalGPUImpl::Instance.Finalize();
    default:
        LogWarn("LogicalGPU::Finalize: Is not supported by current API");
    }
}

}//namespace StraitX::