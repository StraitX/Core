#ifndef STRAITX_SAMPLER_HPP
#define STRAITX_SAMPLER_HPP

#include "core/types.hpp"
#include "core/noncopyable.hpp"

enum class FilteringMode: u8{
    Linear,
    Nearest
};

enum class WrapMode: u8{
    Repeat,
    MirroredRepeat,
    ClampToEdge,
    ClampToBorder
};

struct SamplerProperties{
    FilteringMode MinFiltering = FilteringMode::Linear;
    FilteringMode MagFiltering = FilteringMode::Linear;
    WrapMode WrapU = WrapMode::ClampToEdge;
    WrapMode WrapV = WrapMode::ClampToEdge;
    WrapMode WrapS = WrapMode::ClampToEdge;
};

class Sampler: public NonCopyable{
public:
    Sampler() = default;

    virtual ~Sampler() = default;

    virtual SamplerProperties Properties()const = 0;

    static Sampler *Create(const SamplerProperties &props);
};

#endif//STRAITX_SAMPLER_HPP