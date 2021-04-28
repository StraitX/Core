#include "platform/memory.hpp"
#include "core/log.hpp"
#include "graphics/api/gpu_configuration.hpp"
#include "graphics/api/graphics_api.hpp"

namespace StraitX{

GraphicsAPI *GraphicsAPI::s_Instance = nullptr;
GraphicsAPI::API GraphicsAPI::s_CurrentAPI;

const char *GraphicsAPI::Name(GraphicsAPI::API api){
    switch (api){
    case None:   return "None";
    case Vulkan: return "Vulkan";
    case OpenGL: return "OpenGL";
    }
}

}// namespace StraitX::