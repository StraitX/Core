#ifndef STRAITX_PHYSICAL_GPU_HPP
#define STRAITX_PHYSICAL_GPU_HPP

#include "graphics/api/gpu_configuration.hpp"

namespace StraitX{

struct PhysicalGPU{
    GPUHandle Handle = nullptr;
    GPUVendor Vendor = GPUVendor::Unknown;
    GPUType   Type   = GPUType::Unknown;

    constexpr PhysicalGPU() = default;

    constexpr PhysicalGPU(GPUHandle handle,GPUVendor vendor, GPUType type):
        Handle(handle),
        Vendor(vendor),
        Type(type)
    {}
};

}//namespace StraitX::

#endif // STRAITX_PHYSICAL_GPU_HPP