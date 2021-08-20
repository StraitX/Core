#include "graphics/api/vulkan/texture_impl.hpp"
#include "graphics/api/vulkan/gpu_impl.hpp"

namespace Vk{

VkFormat ToVkFormat(TextureFormat format){
    static VkFormat s_FormatTable[] = {
        VK_FORMAT_UNDEFINED,
        VK_FORMAT_R8G8B8A8_UNORM,
        VK_FORMAT_D24_UNORM_S8_UINT,
        VK_FORMAT_B8G8R8A8_UNORM,
        VK_FORMAT_D32_SFLOAT,
        // XXX pick proper one later
        VK_FORMAT_R8G8B8A8_UNORM
    };
    return s_FormatTable[(size_t)format];
}

VkImageLayout ToVkLayout(TextureLayout layout){
    static VkImageLayout s_LayoutTable[] = {
        VK_IMAGE_LAYOUT_UNDEFINED,
        VK_IMAGE_LAYOUT_GENERAL,
        VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
        VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL,
        VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
        VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
        VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,
        VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL
    };

    return s_LayoutTable[(size_t)layout];
}

VkImageUsageFlags ToVkImageUsage(TextureUsage usage){
    return VkImageUsageFlags(usage);
}

Texture2DImpl::Texture2DImpl(u32 width, u32 height, TextureFormat format, TextureUsage usage, TextureLayout initial_layout){
    m_Format = format;
    m_Usage = usage;
    m_Layout = initial_layout;
    m_Width = width;
    m_Height = height;
    CreateImage();
    CreateImageView();

    // XXX If initial layout is not undefined, do DMA layout translation
}

Texture2DImpl::Texture2DImpl(VkImage image, u32 width, u32 height, TextureFormat image_format, TextureUsage image_usage, TextureLayout image_layout){
    m_Format = image_format;
    m_Usage = image_usage;
    m_Layout = image_layout;
    m_Width = width;
    m_Height = height;
    CreateImageView();
}

Texture2DImpl::~Texture2DImpl(){
    DestroyImageView();

    if(!IsCreatedFromImage())
        DestroyImage();
}

void Texture2DImpl::CreateImage(){
    SX_ASSERT(false);
}

void Texture2DImpl::CreateImageView(){
    VkImageViewCreateInfo view_info;
    view_info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    view_info.pNext = nullptr;
    view_info.flags = 0;
    view_info.image = m_Handle;
    view_info.viewType = VK_IMAGE_VIEW_TYPE_2D;
    view_info.format = ToVkFormat(m_Format);
    view_info.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
    view_info.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
    view_info.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
    view_info.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
    view_info.subresourceRange.aspectMask = IsDepthFormat(m_Format) ? VK_IMAGE_ASPECT_DEPTH_BIT : VK_IMAGE_ASPECT_COLOR_BIT;
    view_info.subresourceRange.baseArrayLayer = 0;
    view_info.subresourceRange.baseMipLevel = 0;
    view_info.subresourceRange.layerCount = 1;
    view_info.subresourceRange.levelCount = 1;

    vkCreateImageView(GPUImpl::s_Instance, &view_info, nullptr, &m_View);
}

void Texture2DImpl::DestroyImageView(){
    vkDestroyImageView(GPUImpl::s_Instance, m_View, nullptr);
}

void Texture2DImpl::DestroyImage(){
    SX_ASSERT(false);
}


}//namespace Vk::