#include "core/log.hpp"
#include "platform/vulkan.hpp"
#include "graphics/vulkan/gpu_texture_impl.hpp"

namespace StraitX{
namespace Vk{

VkFormat FormatTable[] = {
    VK_FORMAT_UNDEFINED,
    VK_FORMAT_B8G8R8A8_UNORM,
    VK_FORMAT_R32_SFLOAT
};

VkImageLayout LayoutTable[] = {
    VK_IMAGE_LAYOUT_UNDEFINED,
    VK_IMAGE_LAYOUT_GENERAL,
    VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
    VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_OPTIMAL,
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
    info.format = FormatTable[(u32)Format];
    info.extent.width = Width;
    info.extent.height = Height;
    info.extent.depth = 1;
    info.mipLevels = 1;
    info.arrayLayers = 1;
    info.samples = VK_SAMPLE_COUNT_1_BIT;
    info.tiling = VK_IMAGE_TILING_OPTIMAL;
    info.usage = Usage;
    info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    info.queueFamilyIndexCount = 0;
    info.pQueueFamilyIndices = nullptr;
    info.initialLayout = LayoutTable[(u32)Layout];

    CoreFunctionAssert(vkCreateImage(Owner->Handle, &info, nullptr, &Handle), VK_SUCCESS, "Vk: GPUTextureImpl: Can't create Image");

    VkMemoryRequirements req;
    vkGetImageMemoryRequirements(Owner->Handle, Handle, &req);

    Memory = Owner->Alloc(req.size, MemoryTypes::VRAM);

    CoreFunctionAssert(vkBindImageMemory(Owner->Handle, Handle, Memory, 0), VK_SUCCESS, "Vk: GPUTextureImpl: can't bind image memory");
}

void GPUTextureImpl::Destroy(){
    vkDestroyImage(Owner->Handle, Handle, nullptr);

    Owner->Free(Memory);
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