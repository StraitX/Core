#include "graphics/api/graphics_api.hpp"
#include "graphics/vulkan/graphics_api_impl.hpp"
#include "graphics/opengl/graphics_api_impl.hpp"

namespace StraitX{

GraphicsAPI *GraphicsAPI::s_Instance = nullptr;

GraphicsAPI::API GraphicsAPI::s_CurrentAPI = GraphicsAPI::None;

static Vk::GraphicsAPIImpl VulkanImpl;
static GL::GraphicsAPIImpl OpenGLImpl;

Result GraphicsAPI::Create(GraphicsAPI::API api){
    switch (api) {
    case API::None: 
        return Result::Failure;
    case API::OpenGL: 
        s_Instance = &OpenGLImpl;
        s_CurrentAPI = API::OpenGL;
        return Result::Success;
    case API::Vulkan: 
        s_Instance = &VulkanImpl;
        s_CurrentAPI = API::Vulkan;
        return Result::Success;
    }
    return Result::Unsupported;
}

}// namespace StraitX::