#include "core/log.hpp"
#include "graphics/api/logical_gpu.hpp"
#include "graphics/api/graphics_api.hpp"
#include "graphics/vulkan/logical_gpu_impl.hpp"
#include "graphics/opengl/logical_gpu_impl.hpp"

namespace StraitX{

LogicalGPU *LogicalGPU::New(const PhysicalGPU &gpu){
    switch (GraphicsAPI::Instance().CurrentAPI()) {
    case GraphicsAPI::Vulkan:
        return new Vk::LogicalGPUImpl(gpu);
    case GraphicsAPI::OpenGL:
        return new GL::LogicalGPUImpl(gpu);
    default:
        LogWarn("LogicalGPU::New: Unsupported API");
        return nullptr;
    }
}

void LogicalGPU::Delete(LogicalGPU *gpu){
    switch (GraphicsAPI::Instance().CurrentAPI()) {
    case GraphicsAPI::Vulkan:
        delete static_cast<Vk::LogicalGPUImpl*>(gpu);
        return;
    case GraphicsAPI::OpenGL:
        delete static_cast<GL::LogicalGPUImpl*>(gpu);
        return;
    default:
        LogWarn("LogicalGPU::Delete: Unsupported API");
    }
}

}//namespace StraitX::