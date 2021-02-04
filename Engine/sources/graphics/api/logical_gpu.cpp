#include "core/log.hpp"
#include "graphics/api/logical_gpu.hpp"
#include "graphics/api/graphics_api.hpp"
#include "graphics/vulkan/logical_gpu_impl.hpp"
#include "graphics/opengl/logical_gpu_impl.hpp"

namespace StraitX{

static GL::LogicalGPUImpl GL_LogicalGPU;
static Vk::LogicalGPUImpl Vk_LogicalGPU;

LogicalGPU *LogicalGPU::s_Instance = nullptr;

Result LogicalGPU::Initialize(const PhysicalGPU &gpu){
    switch (GraphicsAPI::CurrentAPI()) {
    case GraphicsAPI::Vulkan:
        s_Instance = &Vk_LogicalGPU;
        return Vk_LogicalGPU.Initialize(gpu);
    case GraphicsAPI::OpenGL:
        s_Instance = &GL_LogicalGPU;
        return GL_LogicalGPU.Initialize(gpu);
    default:
        LogWarn("LogicalGPU::Initialize: Is not supported by current API");
        return Result::NotFound;
    }
}

void LogicalGPU::Finalize(){
    switch (GraphicsAPI::CurrentAPI()) {
    case GraphicsAPI::Vulkan:
        static_cast<Vk::LogicalGPUImpl*>(s_Instance)->Finalize();
    case GraphicsAPI::OpenGL:
        static_cast<GL::LogicalGPUImpl*>(s_Instance)->Finalize();
    default:
        LogWarn("LogicalGPU::Finalize: Is not supported by current API");
    }
}

}//namespace StraitX::