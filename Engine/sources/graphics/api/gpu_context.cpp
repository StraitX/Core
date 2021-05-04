#include "graphics/api/gpu_context.hpp"

namespace StraitX{

GPUContext::VTable GPUContext::s_VTable;
GPUContext *GPUContext::s_Instance = nullptr;

}//namespace StraitX::