#include "core/algorithm.hpp"
#include "graphics/api/gpu_texture.hpp"

namespace StraitX{

GPUTexture::VTable GPUTexture::s_VTable;


GPUTexture::GPUTexture(GPUTexture &&other){
    Swap(*this, other);
}

GPUTexture &GPUTexture::operator=(GPUTexture &&other){
    m_Handle = other.m_Handle;
    m_ViewHandle = other.m_ViewHandle;
    m_BackingMemory = other.m_BackingMemory;
    m_Width = other.m_Width;
    m_Height = other.m_Height;
    m_Layout = other.m_Layout;
    m_Format = other.m_Format;
    m_Usage = other.m_Usage;

    other.m_Handle = {};
    other.m_ViewHandle = {};
    other.m_BackingMemory = {};
    other.m_Width = 0; 
    other.m_Height = 0;
    other.m_Layout = GPUTexture::Layout::Undefined;
    other.m_Format = GPUTexture::Format::Unknown;
    other.m_Usage = {};
    return *this;
}

bool GPUTexture::IsDepthFormat(GPUTexture::Format format){
    if(format == GPUTexture::Format::Depth24Stencil8)
        return true;
    return false;
}

}//namespace StraitX::