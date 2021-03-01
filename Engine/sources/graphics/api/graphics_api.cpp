#include "graphics/api/graphics_api.hpp"

namespace StraitX{

GraphicsAPI *GraphicsAPI::s_Instance = nullptr;
GraphicsAPI::API GraphicsAPI::s_CurrentAPI = GraphicsAPI::None;

const char *GraphicsAPI::GetName(GraphicsAPI::API api){
    switch (api){
    case None:   return "None";
    case Vulkan: return "Vulkan";
    case OpenGL: return "OpenGL";
    }
}

}// namespace StraitX::