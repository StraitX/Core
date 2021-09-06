#include "graphics/api/vulkan/immediate.hpp"

namespace Vk{


RawVar<Fence> Immediate::s_OpFence;
RawVar<Vk::CommandPoolImpl> Immediate::s_Pool;
CommandBuffer *Immediate::s_CmdBuffer = nullptr;


void Immediate::Initialize(){
    s_OpFence.Construct();
    s_Pool.Construct(QueueFamily::Transfer);

    s_CmdBuffer = s_Pool->Alloc();
}

void Immediate::Finalize(){
    s_Pool->Free(s_CmdBuffer);

    s_Pool.Destruct();
    s_OpFence.Destruct();
}


}//namespace Vk::