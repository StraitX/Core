#ifndef STRAITX_TEXTURE_HPP
#define STRAITX_TEXTURE_HPP

#include "core/noncopyable.hpp"
#include "graphics/api/gpu_texture.hpp"
#include "graphics/api/sampler.hpp"
#include "graphics/image.hpp"

namespace StraitX{

class Texture: public NonCopyable{
private:
    GPUTexture m_GPUTexture;
    Sampler m_Sampler;
public:
    Texture() = default;

    ~Texture();

    constexpr const GPUTexture &GetGPUTexture()const;

    constexpr const Sampler &GetSampler()const;

    constexpr GPUTexture &GetGPUTexture();

    constexpr Sampler &GetSampler();

    Result LoadFromFile(const char *filename, const SamplerProperties &props = {});

    Result LoadFromImage(const Image &image, const SamplerProperties &props = {});

    void Delete();

};

sx_inline Texture::~Texture(){
    if(m_GPUTexture.Handle().U64)
        Delete();
}

constexpr const GPUTexture &Texture::GetGPUTexture()const{
    return m_GPUTexture;
}

constexpr const Sampler &Texture::GetSampler()const{
    return m_Sampler;
}

constexpr GPUTexture &Texture::GetGPUTexture(){
    return m_GPUTexture;
}

constexpr Sampler &Texture::GetSampler(){
    return m_Sampler;
}

}//namespace StraitX::

#endif//STRAITX_TEXTURE_HPP