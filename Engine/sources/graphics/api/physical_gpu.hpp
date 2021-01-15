#ifndef STRAITX_PHYSICAL_GPU_HPP
#define STRAITX_PHYSICAL_GPU_HPP

#include "platform/defs.hpp"
#include "graphics/api/gpu_configuration.hpp"

namespace StraitX{

struct PhysicalGPU{
    const GPUHandle Handle = {};
    const GPUVendor Vendor = GPUVendor::Unknown;
    const GPUType   Type   = GPUType::Unknown;

    const u32 QueueFamiliesCount = 0;

    constexpr PhysicalGPU() = default;

};

}//namespace StraitX::

#endif // STRAITX_PHYSICAL_GPU_HPP