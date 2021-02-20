#include <new>
#include "platform/memory.hpp"
#include "core/algorithm.hpp"
#include "core/log.hpp"
#include "graphics/vulkan/render_pass_impl.hpp"
#include "graphics/vulkan/gpu_texture_impl.hpp"

namespace StraitX{
namespace Vk{

RenderPassImpl::RenderPassImpl(const LogicalGPU &owner, const RenderPassProperties &props):
    Owner(static_cast<const Vk::LogicalGPUImpl *>(&owner))
{
    ArrayPtr<VkAttachmentDescription> attachments((VkAttachmentDescription*)alloca(props.Attachments.Size() * sizeof(VkAttachmentDescription)),props.Attachments.Size());
    ArrayPtr<VkAttachmentReference> references((VkAttachmentReference *)alloca(props.Attachments.Size() * sizeof(VkAttachmentReference)), props.Attachments.Size());

    for(size_t i = 0; i<attachments.Size(); i++){
        //also fill attachment desctiptions array
        Attachments.Push(props.Attachments[i]);

        if(GPUTexture::IsDepthFormat(props.Attachments[i].Format))
            DepthIndex = i;

        attachments[i].flags = 0;
        attachments[i].format = Vk::GPUTextureImpl::s_FormatTable[(size_t)props.Attachments[i].Format];
        attachments[i].samples = Vk::GPUTextureImpl::ToVkSampleCount(props.Attachments[i].Samples);
        attachments[i].loadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        attachments[i].storeOp = VK_ATTACHMENT_STORE_OP_STORE;
        attachments[i].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        attachments[i].stencilStoreOp = VK_ATTACHMENT_STORE_OP_STORE;
        attachments[i].initialLayout = Vk::GPUTextureImpl::s_LayoutTable[(size_t)props.Attachments[i].InitialLayout];
        attachments[i].finalLayout = Vk::GPUTextureImpl::s_LayoutTable[(size_t)props.Attachments[i].FinalLayout];

        references[i].layout = Vk::GPUTextureImpl::s_LayoutTable[(size_t)props.Attachments[i].InPassLayout];
        references[i].attachment = i;
    }

    if(HasDepth()){
        size_t new_depth = attachments.Size() - 1;
        Swap(references[DepthIndex],references[new_depth]);
        DepthIndex = new_depth;
    }

    VkSubpassDescription subpass;
    subpass.flags = 0;
    subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpass.inputAttachmentCount = 0;
    subpass.pInputAttachments = nullptr;//    DepthAttachment is last though depth_index equals to color attachments count
    subpass.colorAttachmentCount = HasDepth() ? DepthIndex : references.Size();
    subpass.pColorAttachments = references.Pointer();
    subpass.pResolveAttachments = nullptr;
    subpass.pDepthStencilAttachment = HasDepth() ? &references[DepthIndex] : nullptr;
    subpass.preserveAttachmentCount = 0;
    subpass.pPreserveAttachments = nullptr;

    VkRenderPassCreateInfo info;
    info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    info.pNext = nullptr;
    info.flags = 0;
    info.attachmentCount = attachments.Size();
    info.pAttachments = attachments.Pointer();
    info.subpassCount = 1;
    info.pSubpasses = &subpass;
    info.dependencyCount = 0;
    info.pDependencies = nullptr;

    CoreFunctionAssert(vkCreateRenderPass(Owner->Handle, &info, nullptr, &Handle),VK_SUCCESS, "Vk: RenderPassImpl: Creation failed");
}

RenderPassImpl::~RenderPassImpl(){
    vkDestroyRenderPass(Owner->Handle, Handle, nullptr);
}

RenderPass *RenderPassImpl::NewImpl(const LogicalGPU &owner, const RenderPassProperties &props){
    return new(Memory::Alloc(sizeof(Vk::RenderPassImpl))) Vk::RenderPassImpl(owner, props);
}
    
void RenderPassImpl::DeleteImpl(RenderPass *pass){
    pass->~RenderPass();
    Memory::Free(pass);
}


}//namespace Vk::
}//namespace StraitX::