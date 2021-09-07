#ifndef STRAITX_VULKAN_IMMEDIATE_HPP
#define STRAITX_VULKAN_IMMEDIATE_HPP

#include "core/raw_var.hpp"
#include "graphics/api/vulkan/command_buffer_impl.hpp"
#include "graphics/api/fence.hpp"

namespace Vk{

class Immediate{
private:
    static RawVar<Fence> s_OpFence;
    static RawVar<Vk::CommandPoolImpl> s_Pool;
    static CommandBuffer *s_CmdBuffer;

    friend class GPUImpl;
private:
    static void Initialize();

    static void Finalize();
public:

    static void Copy(const Buffer *src, const Buffer *dst, size_t size, size_t src_offset, size_t dst_offset){
        s_CmdBuffer->Begin();
        s_CmdBuffer->Copy(src, dst, size, src_offset, dst_offset);
        s_CmdBuffer->End();

        GPU::Execute(s_CmdBuffer, *s_OpFence);
        s_OpFence->WaitAndReset();
    }

    static void Copy(const Buffer *src, const Texture2D *dst, Vector2u src_size, Vector2u dst_offset = {0,0}){
        s_CmdBuffer->Begin();
        s_CmdBuffer->Copy(src, dst, src_size, dst_offset);
        s_CmdBuffer->End();

        GPU::Execute(s_CmdBuffer, *s_OpFence);
        s_OpFence->WaitAndReset();
    }

    static void Copy(const Buffer *src, const Texture2D *dst){
        Copy(src, dst, dst->Size(), {0, 0});
    }

    static void ChangeLayout(Texture2D *texture, TextureLayout new_layout){
        s_CmdBuffer->Begin();
        s_CmdBuffer->ChangeLayout(texture, new_layout);
        s_CmdBuffer->End();

        GPU::Execute(s_CmdBuffer, *s_OpFence);
        s_OpFence->WaitAndReset();
    }
};

}//namespace Vk::

#endif//STRAITX_VULKAN_IMMEDIATE_HPP