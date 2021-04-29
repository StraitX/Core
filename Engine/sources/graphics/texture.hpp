#ifndef STRAITX_TEXTURE_HPP
#define STRAITX_TEXTURE_HPP

#include "core/noncopyable.hpp"
#include "graphics/api/gpu_texture.hpp"
#include "graphics/api/sampler.hpp"
#include "graphics/image.hpp"

namespace StraitX{

struct Texture: public NonCopyable{
    class GPUTexture GPUTexture;
    class Sampler Sampler;

    Texture() = default;

    Texture(const char *filename);

    Texture(const Image &image);

    Texture(Texture &&other);

    Texture &operator=(Texture &&other);

    Result LoadFromFile(const char *filename, const SamplerProperties &props = {});

    Result LoadFromImage(const Image &image, const SamplerProperties &props = {});

    void Delete();

    bool IsEmpty()const;
};

}//namespace StraitX::

#endif//STRAITX_TEXTURE_HPP