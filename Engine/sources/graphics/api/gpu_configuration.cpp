#include "core/assert.hpp"
#include "graphics/api/gpu_configuration.hpp"

namespace StraitX{



GPUVendor VendorIDToVendor(u32 vendor_id){
    switch (vendor_id) {
    case GPU_VENDOR_ID_NVIDIA:      return GPUVendor::Nvidia;
    case GPU_VENDOR_ID_AMD:         return GPUVendor::AMD;
    case GPU_VENDOR_ID_INTEL:       return GPUVendor::Intel;
    case GPU_VENDOR_ID_MICROSOFT:   return GPUVendor::Microsoft;
    default:
        return GPUVendor::Unknown;
    }
}

const char *GPUVendorNameTable[]={
    "Unknown",
    "Nvidia",
    "AMD",
    "Intel",
    "Microsoft"
};

const char *GetName(GPUVendor vendor){
    return GPUVendorNameTable[(u8)vendor];
}

const char *GPUTypeNameTable[]={
    "Unknown",
    "Discrete",
    "Integrated",
    "Virtual",
    "Other"
};

const char *GetName(GPUType type){
    return GPUTypeNameTable[(u8)type];
}

}//namespace StraitX::