#include "graphics/api/gpu_texture.hpp"

namespace StraitX{

GPUTexture::VTable GPUTexture::s_VTable;

bool GPUTexture::IsDepthFormat(GPUTexture::Format format){
    if(format == GPUTexture::Format::Depth24Stencil8)
        return true;
    return false;
}

}//namespace StraitX::