#include "platform/compiler.hpp"
#include "graphics/api/render_pass.hpp"

namespace StraitX{

RenderPass::VTable RenderPass::s_VTable;

RenderPass::RenderPass(const ArrayPtr<const AttachmentDescription> &attachments){

    for(auto &att: attachments){
        if(GPUTexture::IsDepthFormat(att.Format)){
            CoreAssert(m_HasDepth == false, "RenderPass: more than one depth attachment is not supported");
            m_HasDepth = true;
        }
        m_Attachments.Push(att);
    }
}

}//namespace StraitX::