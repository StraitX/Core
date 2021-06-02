#include <new>
#include "platform/memory.hpp"
#include "core/algorithm.hpp"
#include "core/log.hpp"
#include "graphics/vulkan/render_pass_impl.hpp"
#include "graphics/vulkan/gpu_texture_impl.hpp"
#include "graphics/vulkan/gpu.hpp"

namespace StraitX{
namespace Vk{

RenderPassImpl::RenderPassImpl(const RenderPassProperties &props){
    Span<VkAttachmentDescription> attachments((VkAttachmentDescription*)alloca(props.Attachments.Size() * sizeof(VkAttachmentDescription)),props.Attachments.Size());
    Span<VkAttachmentReference> references((VkAttachmentReference *)alloca(props.Attachments.Size() * sizeof(VkAttachmentReference)), props.Attachments.Size());

    for(size_t i = 0; i<attachments.Size(); i++){
        //also fill attachment desctiptions array
        m_Attachments.Push(props.Attachments[i]);

        if(IsDepthFormat(props.Attachments[i].Format))
            m_DepthIndex = i;

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

    CoreFunctionAssert(vkCreateRenderPass(GPU::Get().Handle(), &info, nullptr, &m_Handle),VK_SUCCESS, "Vk: RenderPassImpl: Creation failed");
}

RenderPassImpl::~RenderPassImpl(){
    vkDestroyRenderPass(GPU::Get().Handle(), m_Handle, nullptr);
}

RenderPass *RenderPassImpl::NewImpl(const RenderPassProperties &props){
    return new(Memory::Alloc(sizeof(Vk::RenderPassImpl))) Vk::RenderPassImpl(props);
}
    
void RenderPassImpl::DeleteImpl(RenderPass *pass){
    pass->~RenderPass();
    Memory::Free(pass);
}


}//namespace Vk::
}//namespace StraitX::