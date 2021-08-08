#include "core/algorithm.hpp"
#include "core/assert.hpp"
#include "graphics/texture.hpp"
#include "graphics/cpu_texture.hpp"
#include "graphics/dma.hpp"
#include "graphics/image.hpp"

Texture2D::VTable Texture2D::s_VTable;

Texture2D::Texture2D(Texture2D&& other)noexcept {
    *this = Move(other);
}

Texture2D::Texture2D(const char *filename, const SamplerProperties &props){
	SX_CORE_CALL_ASSERT(New(filename, props), Result::Success, "Can't load image from file");
}

Texture2D::Texture2D(const Image &image, const SamplerProperties &props){
	New(image, props);
}

Texture2D& Texture2D::operator=(Texture2D&& other) noexcept{
    SX_CORE_ASSERT(IsEmpty(), "Texture2D: Can't move into non-empty object");
    Texture::operator=(Move(other));
    m_Width = other.m_Width;
    m_Height = other.m_Height;
    m_Sampler = Move(other.m_Sampler);

    other.m_Width = 0;
    other.m_Height = 0;

    return *this;
}

Result Texture2D::New(const char *filename, const SamplerProperties &props){
    Image image;

    auto result = image.LoadFromFile(filename);

    if(!result)return result;

    New(image, props);

	return Result::Success;
}

void Texture2D::New(const Image &image, const SamplerProperties &props){
	New(image.Width(), image.Height(), ToTextureFormat(image.Format()), TextureUsageBits((int)TextureUsageBits::Sampled | (int)TextureUsageBits::TransferDst), props);

	CPUTexture staging;
    staging.New(image.Width(), image.Height(), ToTextureFormat(image.Format()), image.Data());
        DMA::ChangeLayout(*this, TextureLayout::TransferDstOptimal);
        DMA::Copy(staging, *this);
        DMA::ChangeLayout(*this, TextureLayout::ShaderReadOnlyOptimal);
    staging.Delete();
}