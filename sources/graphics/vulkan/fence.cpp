#include "graphics/vulkan/fence.hpp"
#include "graphics/vulkan/gpu.hpp"
#include "graphics/vulkan/debug.hpp"

namespace Vk{

Fence::Fence(){
    VkFenceCreateInfo info;
    info.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    info.pNext = nullptr;
    info.flags = 0;
    SX_VK_ASSERT(vkCreateFence(GPU::Get().Handle(), &info, nullptr, &Handle), "Vk: Fence: Creation Failed");
}

Fence::~Fence(){
    vkDestroyFence(GPU::Get().Handle(), Handle, nullptr);
}

void Fence::Wait()const{
    vkWaitForFences(GPU::Get().Handle(), 1, &Handle, true, ~0ull);
}

void Fence::Reset()const{
    vkResetFences(GPU::Get().Handle(), 1, &Handle);
}

void Fence::WaitAndReset()const{
	Wait();
	Reset();
}

}//namespace Vk::