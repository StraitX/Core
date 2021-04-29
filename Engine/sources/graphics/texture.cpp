#include "graphics/api/cpu_texture.hpp"
#include "graphics/api/dma.hpp"
#include "graphics/texture.hpp"

namespace StraitX{

Texture::Texture(const char *filename){
    CoreFunctionAssert(
        LoadFromFile(filename), Result::Success, 
        "Texture: Can't load texture from file"
    );
}

Texture::Texture(const Image &image){
    CoreFunctionAssert(
        LoadFromImage(image), Result::Success, 
        "Texture: Can't load texture from image"
    );
}

Texture::Texture(Texture &&other){
    *this = Move(other);
}

Texture::~Texture(){
    if(!GPUTexture.IsEmpty())
        Delete();
}

Texture &Texture::operator=(Texture &&other){
    CoreAssert(IsEmpty(), "Texture: Can't move into non-empty object");
    GPUTexture = Move(other.GPUTexture);
    Sampler = Move(other.Sampler);
    return *this;
}

Result Texture::LoadFromFile(const char *filename, const SamplerProperties &props){
    Image image;

    auto result = image.LoadFromFile(filename);

    if(!result)return result;

    return LoadFromImage(image, props);
}

Result Texture::LoadFromImage(const Image &image, const SamplerProperties &props){
    if(image.Width() == 0 || image.Height() == 0)return Result::NullObject;

    GPUTexture.New(ToTextureFormat(image.Format()), GPUTexture::UsageBits::Sampled | GPUTexture::UsageBits::TransferDst, image.Width(), image.Height());

    CPUTexture staging;
    staging.New(image.Width(), image.Height(), ToTextureFormat(image.Format()), image.Data());
        DMA::ChangeLayout(GPUTexture, GPUTexture::Layout::TransferDstOptimal);
        DMA::Copy(staging, GPUTexture);
        DMA::ChangeLayout(GPUTexture, GPUTexture::Layout::ShaderReadOnlyOptimal);
    staging.Delete();

    Sampler.New(props);
    
    return Result::Success;
}

void Texture::Delete(){
    Sampler.Delete();
    GPUTexture.Delete();
}

bool Texture::IsEmpty()const{
    return GPUTexture.IsEmpty() && Sampler.IsEmpty();
}

}//namespace StraitX::