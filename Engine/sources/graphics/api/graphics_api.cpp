#include "platform/memory.hpp"
#include "core/log.hpp"
#include "graphics/api/gpu_configuration.hpp"
#include "graphics/api/graphics_api.hpp"

namespace StraitX{

GraphicsAPI *GraphicsAPI::s_Instance = nullptr;
GraphicsAPI::API GraphicsAPI::s_CurrentAPI = GraphicsAPI::None;

PhysicalGPU GraphicsAPI::PickBestGPU(){
    auto devices_count = GetPhysicalGPUCount();

    if(!devices_count){
        LogWarn("GraphicsAPI: can't find any PhysicalGPU on this system");
        return PhysicalGPU();
    }

    auto *devices = (PhysicalGPU*)alloca(sizeof(PhysicalGPU) * devices_count);

    GetPhysicalGPUs(devices);

    return devices[0];
}

const char *GraphicsAPI::GetName(GraphicsAPI::API api){
    switch (api){
    case None:   return "None";
    case Vulkan: return "Vulkan";
    case OpenGL: return "OpenGL";
    }
}

}// namespace StraitX::