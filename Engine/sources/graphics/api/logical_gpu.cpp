#include "core/log.hpp"
#include "graphics/api/logical_gpu.hpp"
#include "graphics/api/graphics_api.hpp"
#include "graphics/vulkan/logical_gpu_impl.hpp"
#include "graphics/opengl/logical_gpu_impl.hpp"

namespace StraitX{

LogicalGPU *LogicalGPU::s_Instance = nullptr;

}//namespace StraitX::