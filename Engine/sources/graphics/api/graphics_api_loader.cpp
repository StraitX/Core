#include "core/log.hpp"
#include "graphics/api/graphics_api_loader.hpp"

#include "graphics/api/graphics_api.hpp"
#include "graphics/vulkan/graphics_api_impl.hpp"
#include "graphics/opengl/graphics_api_impl.hpp"

#include "graphics/api/logical_gpu.hpp"
#include "graphics/vulkan/logical_gpu_impl.hpp"
#include "graphics/opengl/logical_gpu_impl.hpp"

#include "graphics/api/gpu_context.hpp"
#include "graphics/vulkan/gpu_context_impl.hpp"
#include "graphics/opengl/gpu_context_impl.hpp"

#include "graphics/api/swapchain.hpp"
#include "graphics/vulkan/swapchain_impl.hpp"
#include "graphics/opengl/swapchain_impl.hpp"

namespace StraitX{

static GraphicsAPI *const GraphicsAPITable[GraphicsAPI::ElementsCount] = {
    nullptr,
    &Vk::GraphicsAPIImpl::Instance,
    &GL::GraphicsAPIImpl::Instance
};

static LogicalGPU *const LogicalGPUTable[GraphicsAPI::ElementsCount] = {
    nullptr,
    &Vk::LogicalGPUImpl::Instance,
    &GL::LogicalGPUImpl::Instance
};

static GPUContext::VTable GPUContextTable[GraphicsAPI::ElementsCount] = {
    {nullptr, nullptr},
    {&Vk::GPUContextImpl::NewImpl, &Vk::GPUContextImpl::DeleteImpl},
    {&GL::GPUContextImpl::NewImpl, &GL::GPUContextImpl::DeleteImpl}
};

static Swapchain::VTable SwapchainTable[GraphicsAPI::ElementsCount] = {
    {nullptr, nullptr},
    {&Vk::SwapchainImpl::NewImpl, &Vk::SwapchainImpl::DeleteImpl},
    {&GL::SwapchainImpl::NewImpl, &GL::SwapchainImpl::DeleteImpl}
};

Result GraphicsAPILoader::Load(GraphicsAPI::API api){
    if(api != GraphicsAPI::OpenGL && api != GraphicsAPI::Vulkan){
        LogError("GraphicsAPILoader::Load: Unknown API: %",(int)api);
        return Result::Unsupported;
    }

    GraphicsAPI::s_Instance = GraphicsAPITable[api];

    LogicalGPU::s_Instance = LogicalGPUTable[api];

    GPUContext::s_VTable = GPUContextTable[api];

    Swapchain::s_VTable = SwapchainTable[api];

    return Result::Success;
}

}//namespace StraitX::