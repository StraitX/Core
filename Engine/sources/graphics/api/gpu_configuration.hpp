#ifndef STRAITX_GPU_CONFIGURATION_HPP
#define STRAITX_GPU_CONFIGURATION_HPP

#include "platform/types.hpp"

#define GPU_VENDOR_ID_NVIDIA    0x10DE
#define GPU_VENDOR_ID_AMD       0x1002
#define GPU_VENDOR_ID_INTEL     0x8086
#define GPU_VENDOR_ID_MICROSOFT 0x1414

namespace StraitX{

enum class GPUVendor: u8{
    Unknown = 0,
    Nvidia,
    AMD,
    Intel,
    Microsoft
};

enum class GPUType: u8{
    Unknown = 0,
    Discrete,
    Integrated,
    Virtual,
    Other
};

typedef void *GPUHandle;

GPUVendor VendorIDToVendor(u32 vendor_id);

const char *GetName(GPUVendor vendor);
const char *GetName(GPUType type);

}//namespace StraitX::

#endif //STRAITX_GPU_CONFIGURATION_HPP