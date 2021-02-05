#include "core/log.hpp"
#include "graphics/api/graphics_api_loader.hpp"
#include "graphics/api/graphics_api.hpp"
#include "graphics/opengl/graphics_api_impl.hpp"
#include "graphics/vulkan/graphics_api_impl.hpp"

#include "graphics/api/logical_gpu.hpp"
#include "graphics/opengl/logical_gpu_impl.hpp"
#include "graphics/vulkan/logical_gpu_impl.hpp"

namespace StraitX{

static GraphicsAPI *GraphicsAPITable[GraphicsAPI::ElementsCount] = {
    nullptr,
    &Vk::GraphicsAPIImpl::Instance,
    &GL::GraphicsAPIImpl::Instance
};

static LogicalGPU *LogicalGPUTable[GraphicsAPI::ElementsCount] = {
    nullptr,
    &Vk::LogicalGPUImpl::Instance,
    &GL::LogicalGPUImpl::Instance
};

Result GraphicsAPILoader::Load(GraphicsAPI::API api){
    if(api != GraphicsAPI::OpenGL && api != GraphicsAPI::Vulkan){
        LogError("GraphicsAPILoader::Load: Unknown API: %",(int)api);
        return Result::Unsupported;
    }

    GraphicsAPI::s_Instance = GraphicsAPITable[api];

    LogicalGPU::s_Instance = LogicalGPUTable[api];

    return Result::Success;
}

}//namespace StraitX::