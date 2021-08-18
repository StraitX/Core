#ifndef STRAITX_GPU_HPP
#define STRAITX_GPU_HPP

#include "core/result.hpp"

enum class GPUType{
    Unknown = 0,
    Virtual,
    Integrated,
    Discrete
};

enum class GPUVendor{
    Unknown = 0,
    Microsoft,
    Nvidia,
    Intel,
    AMD
};

struct GPUImpl{
    GPUType Type = GPUType::Unknown;
    GPUVendor Vendor = GPUVendor::Unknown;

    virtual Result Initialize() = 0;

    virtual void Finalize() = 0;

    //virtual void Schedule(const GPUCommandBuffer &buffer);
};

class GPU{
private:
    static GPUImpl *s_Impl;
public:
    static Result Initialize();

    static void Finalize();

    static GPUType Type();

    static GPUVendor Vendor();

    static bool IsInitialized(){
        return s_Impl;
    }

    //static void Schedule(const GPUCommandBuffer &buffer);
};

#endif//STRAITX_GPU_HPP