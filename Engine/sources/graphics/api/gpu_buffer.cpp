#include "core/log.hpp"
#include "graphics/api/gpu_buffer.hpp"
#include "graphics/api/graphics_api.hpp"
#include "graphics/vulkan/gpu_buffer_impl.hpp"
#include "graphics/opengl/gpu_buffer_impl.hpp"


namespace StraitX{

GPUBuffer::VTable GPUBuffer::s_VTable;

}//namespace StraitX::