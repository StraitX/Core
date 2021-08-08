#ifndef STRAITX_GPU_CONFIGURATION_HPP
#define STRAITX_GPU_CONFIGURATION_HPP

#include "core/types.hpp"

#define GPU_VENDOR_ID_NVIDIA    0x10DE
#define GPU_VENDOR_ID_AMD       0x1002
#define GPU_VENDOR_ID_INTEL     0x8086
#define GPU_VENDOR_ID_MICROSOFT 0x1414

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

enum class GPUQueueType: u8{
    Unknown   = 0x00,
    Transfer  = 0x01,
    Compute   = 0x02,
    Graphics  = 0x04
};

enum class GPUMemoryType: u8{
    VRAM = 0,
    DynamicVRAM = 1
};

union GPUHandle{
    void *Pointer;
    u64   U64;
};

union GPUResourceHandle{
    void *Pointer;
    u64 U64;
    u32 U32;
};

GPUVendor VendorIDToVendor(u32 vendor_id);

const char *GetName(GPUVendor vendor);

const char *GetName(GPUType type);

#endif //STRAITX_GPU_CONFIGURATION_HPP