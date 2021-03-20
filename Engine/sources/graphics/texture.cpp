#include "graphics/api/cpu_texture.hpp"
#include "graphics/api/dma.hpp"
#include "graphics/texture.hpp"

namespace StraitX{

Result Texture::LoadFromFile(const char *filename, const SamplerProperties &props){
    Image image;

    auto result = image.LoadFromFile(filename);

    if(!result)return result;

    return LoadFromImage(image, props);
}

Result Texture::LoadFromImage(const Image &image, const SamplerProperties &props){
    if(image.Width() == 0 || image.Height() == 0)return Result::NullObject;

    m_GPUTexture.New(ToTextureFormat(image.Format()), GPUTexture::UsageBits::Sampled | GPUTexture::UsageBits::TransferDst, image.Width(), image.Height());

    CPUTexture staging;
    staging.New(image.Width(), image.Height(), ToTextureFormat(image.Format()), image.Data());
        DMA::ChangeLayout(m_GPUTexture, GPUTexture::Layout::TransferDstOptimal);
        DMA::Copy(staging, m_GPUTexture);
        DMA::ChangeLayout(m_GPUTexture, GPUTexture::Layout::ShaderReadOnlyOptimal);
    staging.Delete();

    m_Sampler.New(props);
    
    return Result::Success;
}

void Texture::Delete(){
    m_Sampler.Delete();
    m_GPUTexture.Delete();
}

}//namespace StraitX::