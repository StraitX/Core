#ifndef STRAITX_VULKAN_GPU_TEXTURE_IMPL_HPP
#define STRAITX_VULKAN_GPU_TEXTURE_IMPL_HPP

#include "graphics/api/gpu_texture.hpp"
#include "graphics/vulkan/logical_gpu_impl.hpp"

namespace StraitX{
namespace Vk{

struct GPUTextureImpl{
    Vk::LogicalGPUImpl *const Owner;
    VkImage &Handle;
    VkDeviceMemory &Memory;
    u32 &Width;
    u32 &Height;
    GPUTexture::Layout &Layout;
    GPUTexture::Format &Format;
    GPUTexture::Usage  &Usage;


    sx_inline GPUTextureImpl(GPUTexture &texture):
        Owner(static_cast<Vk::LogicalGPUImpl *>(texture.m_Owner)),
        Handle(reinterpret_cast<VkImage&>(texture.m_Handle.U64)),
        Memory(reinterpret_cast<VkDeviceMemory&>(texture.m_BackingMemory)),
        Width(texture.m_Width),
        Height(texture.m_Height),
        Layout(texture.m_Layout),
        Format(texture.m_Format),
        Usage(texture.m_Usage)
    {}

    sx_inline void Create(GPUTexture::Format format, GPUTexture::Usage usage, u32 width, u32 height);

    sx_inline void Destroy();

    static void NewImpl(GPUTexture &texture, GPUTexture::Format format, GPUTexture::Usage usage, u32 width, u32 height);

    static void DeleteImpl(GPUTexture &texture);

};

}//namespace Vk::
}//namespace StraitX::

#endif//STRAITX_VULKAN_GPU_TEXTURE_IMPL_HPP