#include "graphics/api/vulkan/render_pass_impl.hpp"
#include "graphics/api/vulkan/texture_impl.hpp"
#include "graphics/api/vulkan/gpu_impl.hpp"
#include "graphics/api/vulkan/debug.hpp"
#include "core/assert.hpp"
#include "core/os/memory.hpp"
#include "core/algorithm.hpp"

namespace Vk{

RenderPassImpl::RenderPassImpl(const RenderPassProperties &props){
    SX_CORE_ASSERT(props.Attachments.Size() <= MaxAttachmentsCount, "Vk: RenderPassImpl: exceeded MaxAttachmentCount");

    Span<VkAttachmentDescription> attachments(SX_STACK_ARRAY_ALLOC(VkAttachmentDescription, props.Attachments.Size()),props.Attachments.Size());
    Span<VkAttachmentReference> references(SX_STACK_ARRAY_ALLOC(VkAttachmentReference, props.Attachments.Size()), props.Attachments.Size());

    for(size_t i = 0; i<attachments.Size(); i++){
        //also fill attachment desctiptions array
        m_Attachments.Add(props.Attachments[i]);

        if(IsDepthFormat(props.Attachments[i].Format))
            m_DepthIndex = i;

        attachments[i].flags = 0;
        attachments[i].format = ToVkFormat(props.Attachments[i].Format);
        attachments[i].samples = VkSampleCountFlagBits(ToVkSamplesCount(props.Attachments[i].Samples));
        attachments[i].loadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        attachments[i].storeOp = VK_ATTACHMENT_STORE_OP_STORE;
        attachments[i].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        attachments[i].stencilStoreOp = VK_ATTACHMENT_STORE_OP_STORE;
        attachments[i].initialLayout = ToVkLayout(props.Attachments[i].InitialLayout);
        attachments[i].finalLayout = ToVkLayout(props.Attachments[i].FinalLayout);

        references[i].layout = ToVkLayout(props.Attachments[i].InPassLayout);
        references[i].attachment = i;
    }

    if(HasDepth()){
        size_t new_depth = attachments.Size() - 1;
        Swap(references[m_DepthIndex],references[new_depth]);
        m_DepthIndex = new_depth;
    }

    VkSubpassDescription subpass;
    subpass.flags = 0;
    subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpass.inputAttachmentCount = 0;
    subpass.pInputAttachments = nullptr;//    DepthAttachment is last though depth_index equals to color attachments count
    subpass.colorAttachmentCount = HasDepth() ? m_DepthIndex : references.Size();
    subpass.pColorAttachments = references.Pointer();
    subpass.pResolveAttachments = nullptr;
    subpass.pDepthStencilAttachment = HasDepth() ? &references[m_DepthIndex] : nullptr;
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

    SX_VK_ASSERT(vkCreateRenderPass(GPUImpl::s_Instance, &info, nullptr, &m_Handle), "Vk: RenderPassImpl: Creation failed");
}

RenderPassImpl::~RenderPassImpl(){
    vkDestroyRenderPass(GPUImpl::s_Instance, m_Handle, nullptr);
}

ConstSpan<AttachmentDescription> RenderPassImpl::Attachments()const{
    return m_Attachments;
}

}//namespace Vk::