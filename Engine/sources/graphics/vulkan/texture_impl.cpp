#include <cmath>
#include "core/log.hpp"
#include "graphics/vulkan/gpu.hpp"
#include "graphics/vulkan/memory_allocator.hpp"
#include "graphics/vulkan/texture_impl.hpp"
#include "graphics/vulkan/debug.hpp"

namespace Vk{

VkFormat TextureImpl::s_FormatTable[] = {
    VK_FORMAT_UNDEFINED,
    VK_FORMAT_R8G8B8A8_UNORM,
    VK_FORMAT_D24_UNORM_S8_UINT,
    VK_FORMAT_B8G8R8A8_UNORM,
    VK_FORMAT_D32_SFLOAT
};

VkImageLayout TextureImpl::s_LayoutTable[] = {
    VK_IMAGE_LAYOUT_UNDEFINED,
    VK_IMAGE_LAYOUT_GENERAL,
    VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
    VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL,
    VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
    VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
    VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,
    VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL
};

VkSampleCountFlagBits TextureImpl::ToVkSampleCount(SamplePoints samples){
    return static_cast<VkSampleCountFlagBits>((u32)std::pow(2, (u32)samples));
}

Texture2DImpl::Texture2DImpl(Texture2D &texture):
    Handle(reinterpret_cast<VkImage&>(texture.m_Handle.U64)),
    ViewHandle(reinterpret_cast<VkImageView&>(texture.m_ViewHandle.U64)),
    Memory(reinterpret_cast<VkDeviceMemory&>(texture.m_BackingMemory)),
    Width(texture.m_Width),
    Height(texture.m_Height),
    Layout(texture.m_Layout),
    Format(texture.m_Format),
    Usage(texture.m_Usage)
{}

void Texture2DImpl::Create(u32 width, u32 height, TextureFormat format, TextureUsageBits usage){
    SX_CORE_ASSERT(format != TextureFormat::Unknown,"GPUTexture: Can't be created with Format::Unknown");

    VkImageCreateInfo info;
    info.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    info.pNext = nullptr;
    info.flags = 0;
    info.imageType = VK_IMAGE_TYPE_2D;
    info.format = TextureImpl::s_FormatTable[(size_t)format];
    info.extent.width = width;
    info.extent.height = height;
    info.extent.depth = 1;
    info.mipLevels = 1;
    info.arrayLayers = 1;
    info.samples = TextureImpl::ToVkSampleCount(SamplePoints::Samples_1);
    info.tiling = VK_IMAGE_TILING_OPTIMAL;
    info.usage = (u32)usage;
    info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    info.queueFamilyIndexCount = 0;
    info.pQueueFamilyIndices = nullptr;
    info.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;// should be undefined by the spec

    SX_VK_ASSERT(vkCreateImage(GPU::Get().Handle(), &info, nullptr, &Handle), "Vk: Texture2DImpl: Can't create VkImage");

    VkMemoryRequirements req;
    vkGetImageMemoryRequirements(GPU::Get().Handle(), Handle, &req);

    Memory = MemoryAllocator::Alloc(req.size, MemoryType::VRAM);

    SX_VK_ASSERT(vkBindImageMemory(GPU::Get().Handle(), Handle, Memory, 0), "Vk: Texture2DImpl: can't bind image memory");

    CreateWithImage(Handle, width, height, TextureLayout::Undefined, format, usage);
}

void Texture2DImpl::CreateWithImage(VkImage image, u32 width, u32 height, TextureLayout layout, TextureFormat format, TextureUsageBits usage){
    Handle = image;
    ViewHandle = VK_NULL_HANDLE;
    //Memory = image should already have binded memory
    Width = width;
    Height = height;
    Layout = layout;
    Format = format;
    Usage = usage;

    VkImageViewCreateInfo view_info;
    view_info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    view_info.pNext = nullptr;
    view_info.flags = 0;
    view_info.image = Handle;
    view_info.viewType = VK_IMAGE_VIEW_TYPE_2D;
    view_info.format = TextureImpl::s_FormatTable[(size_t)Format];
    view_info.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
    view_info.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
    view_info.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
    view_info.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
    view_info.subresourceRange.aspectMask = IsDepthFormat(Format) ? VK_IMAGE_ASPECT_DEPTH_BIT : VK_IMAGE_ASPECT_COLOR_BIT;
    view_info.subresourceRange.baseArrayLayer = 0;
    view_info.subresourceRange.baseMipLevel = 0;
    view_info.subresourceRange.layerCount = 1;
    view_info.subresourceRange.levelCount = 1;

    SX_VK_ASSERT(vkCreateImageView(GPU::Get().Handle(), &view_info, nullptr, &ViewHandle), "Vk: Texture2DImpl: Can't create VkImageView");
}

void Texture2DImpl::Destroy(){
    DestroyWithoutImage();

    vkDestroyImage(GPU::Get().Handle(), Handle, nullptr);

    MemoryAllocator::Free(Memory);
}

void Texture2DImpl::DestroyWithoutImage(){
    vkDestroyImageView(GPU::Get().Handle(), ViewHandle, nullptr);
}

void Texture2DImpl::NewImpl(Texture2D &texture, u32 width, u32 height, TextureFormat format, TextureUsageBits usage){
    Texture2DImpl(texture).Create(width, height, format, usage);
}

void Texture2DImpl::DeleteImpl(Texture2D &texture){
    Texture2DImpl(texture).Destroy();
}

}//namespace Vk::