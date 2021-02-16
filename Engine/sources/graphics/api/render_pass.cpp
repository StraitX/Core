#include "platform/compiler.hpp"
#include "graphics/api/render_pass.hpp"

namespace StraitX{

RenderPass::VTable RenderPass::s_VTable;

RenderPass::RenderPass(const ArrayPtr<const AttachmentDescription> &attachments){
#ifdef SX_DEBUG
    bool has_depth = false;
#endif
    for(auto &att: attachments){
#ifdef SX_DEBUG
        if(GPUTexture::IsDepthFormat(att.Format)){
            CoreAssert(has_depth == false, "RenderPass: more than one depth attachment is not supported");
            has_depth = true;
        }
#endif
        m_Attachments.Push(att);
    }
}

}//namespace StraitX::