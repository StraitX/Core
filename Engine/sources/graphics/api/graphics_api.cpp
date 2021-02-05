#include "graphics/api/graphics_api.hpp"
#include "graphics/vulkan/graphics_api_impl.hpp"
#include "graphics/opengl/graphics_api_impl.hpp"
#include "graphics/api/logical_gpu.hpp"
#include "graphics/opengl/logical_gpu_impl.hpp"

namespace StraitX{

GraphicsAPI *GraphicsAPI::s_Instance = nullptr;

}// namespace StraitX::