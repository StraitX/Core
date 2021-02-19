#include <cmath>
#include "core/log.hpp"
#include "platform/vulkan.hpp"
#include "graphics/vulkan/gpu_texture_impl.hpp"

namespace StraitX{
namespace Vk{

VkFormat GPUTextureImpl::s_FormatTable[] = {
    VK_FORMAT_UNDEFINED,
    VK_FORMAT_B8G8R8A8_UNORM,
    VK_FORMAT_D24_UNORM_S8_UINT
};

VkImageLayout GPUTextureImpl::s_LayoutTable[] = {
    VK_IMAGE_LAYOUT_UNDEFINED,
    VK_IMAGE_LAYOUT_GENERAL,
    VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
    VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL,
    VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
    VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
    VK_IMAGE_LAYOUT_PRESENT_SRC_KHR
};



void GPUTextureImpl::Create(GPUTexture::Format format, GPUTexture::Usage usage, u32 width, u32 height){
    CoreAssert(format != GPUTexture::Format::Unknown,"GPUTexture: Can't be created with Format::Unknown");

    Width = width;
    Height = height;
    Layout = GPUTexture::Layout::Undefined;
    Format = format;
    Usage = usage;

    LogTrace("TexUsage: %", usage);
    VkImageCreateInfo info;
    info.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    info.pNext = nullptr;
    info.flags = 0;
    info.imageType = VK_IMAGE_TYPE_2D;
    info.format = s_FormatTable[(u32)Format];
    info.extent.width = Width;
    info.extent.height = Height;
    info.extent.depth = 1;
    info.mipLevels = 1;
    info.arrayLayers = 1;
    info.samples = ToVkSampleCount(SamplePoints::Samples_1);
    info.tiling = VK_IMAGE_TILING_OPTIMAL;
    info.usage = Usage;
    info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    info.queueFamilyIndexCount = 0;
    info.pQueueFamilyIndices = nullptr;
    info.initialLayout = s_LayoutTable[(u32)Layout];

    CoreFunctionAssert(vkCreateImage(Owner->Handle, &info, nullptr, &Handle), VK_SUCCESS, "Vk: GPUTextureImpl: Can't create VkImage");

    VkMemoryRequirements req;
    vkGetImageMemoryRequirements(Owner->Handle, Handle, &req);

    Memory = Owner->Alloc(req.size, MemoryTypes::VRAM);

    CoreFunctionAssert(vkBindImageMemory(Owner->Handle, Handle, Memory, 0), VK_SUCCESS, "Vk: GPUTextureImpl: can't bind image memory");

    ViewHandle = CreateImageView(Owner, Handle, Format, Width, Height);
}

void GPUTextureImpl::CreateFromImage(VkImage image, GPUTexture::Format format, u32 width, u32 height){
    Handle = image;
    ViewHandle = CreateImageView(Owner, image, format, width, height);
    Memory = VK_NULL_HANDLE;
    Width = width;
    Height = height;
    Layout = GPUTexture::Layout::Undefined;
    Usage = GPUTexture::UsageBits::Sampled;
    Format = format;
}

void GPUTextureImpl::Destroy(){
    DestroyImageView(Owner, ViewHandle);

    if(Memory){
        vkDestroyImage(Owner->Handle, Handle, nullptr);

        Owner->Free(Memory);
    }
}

VkImageView GPUTextureImpl::CreateImageView(const Vk::LogicalGPUImpl *owner, VkImage image, GPUTexture::Format format, u32 width, u32 height){
    VkImageViewCreateInfo view_info;
    view_info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    view_info.pNext = nullptr;
    view_info.flags = 0;
    view_info.image = image;
    view_info.viewType = VK_IMAGE_VIEW_TYPE_2D;
    view_info.format = GPUTextureImpl::s_FormatTable[(size_t)format];
    view_info.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
    view_info.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
    view_info.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
    view_info.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
    view_info.subresourceRange.aspectMask = GPUTexture::IsDepthFormat(format) ? VK_IMAGE_ASPECT_DEPTH_BIT : VK_IMAGE_ASPECT_COLOR_BIT;
    view_info.subresourceRange.baseArrayLayer = 0;
    view_info.subresourceRange.baseMipLevel = 0;
    view_info.subresourceRange.layerCount = 1;
    view_info.subresourceRange.levelCount = 1;

    VkImageView handle;
    CoreFunctionAssert(vkCreateImageView(owner->Handle, &view_info, nullptr, &handle), VK_SUCCESS, "Vk: GPUTextureImpl: Can't create VkImageView");
    
    return handle;
}

void GPUTextureImpl::DestroyImageView(const Vk::LogicalGPUImpl *owner, VkImageView handle){
    vkDestroyImageView(owner->Handle, handle, nullptr);
}

VkSampleCountFlagBits GPUTextureImpl::ToVkSampleCount(SamplePoints samples){
    return static_cast<VkSampleCountFlagBits>((u32)std::pow(2, (u32)samples));
}
void GPUTextureImpl::NewImpl(GPUTexture &texture, GPUTexture::Format format, GPUTexture::Usage usage, u32 width, u32 height){
    GPUTextureImpl impl(texture);
    impl.Create(format, usage, width, height);
}

void GPUTextureImpl::DeleteImpl(GPUTexture &texture){
    GPUTextureImpl impl(texture);
    impl.Destroy();
}

}//namespace Vk::
}//namespace StraitX::