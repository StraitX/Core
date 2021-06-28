#ifndef STRAITX_VULKAN_GRAPHICS_CONTEXT_IMPL_HPP
#define STRAITX_VULKAN_GRAPHICS_CONTEXT_IMPL_HPP

#include "core/raw_var.hpp"
#include "graphics/api/graphics_context.hpp"
#include "graphics/vulkan/swapchain.hpp"
#include "graphics/vulkan/command_buffer.hpp"
#include "graphics/vulkan/fence.hpp"
#include "graphics/vulkan/semaphore.hpp"

namespace StraitX{
namespace Vk{

class GraphicsPipelineImpl;

class GraphicsContextImpl: public GraphicsContext{
private:
	VkInstance m_Instance = VK_NULL_HANDLE;
	VkDebugUtilsMessengerEXT m_Messenger = VK_NULL_HANDLE;

	RawVar<Vk::Swapchain> m_Swapchain;
	RawVar<Vk::Semaphore> m_PresentSemaphore;

	RawVar<Vk::CommandBuffer> m_CommandBuffer;

	RawVar<Vk::Fence> m_SignalFence;

	const Vk::GraphicsPipelineImpl *m_PipelineBindPoint = nullptr;
	const Vk::RenderPassImpl *m_CurrentRenderPass = nullptr;
	const Vk::FramebufferImpl *m_CurrentFramebuffer = nullptr;
public:
	static GraphicsContextImpl s_Instance;
public:
	virtual Result Initialize(const Window &window)override;

	virtual void Finalize()override;

	virtual void ExecuteCmdBuffer(const GPUCommandBuffer &cmd_buffer)override;

	virtual void SwapBuffers()override;

	virtual const Framebuffer *CurrentFramebuffer()override;

	virtual const RenderPass *FramebufferPass()override;

	VkInstance Instance()const;
private:
	static GPUType VkTypeToGPUType(u32 type);

	static int GetPriority(GPUType type);

    static VkPhysicalDevice PickBestPhysicalDevice(VkInstance instance);

    static bool CheckLayers(const Span<const char *> &layers);

    static bool CheckExtensions(const Span<const char *> &extensions);
};

SX_INLINE VkInstance GraphicsContextImpl::Instance()const{
	return m_Instance;
}

}//namespace Vk::
}//namespace StraitX::

#endif//STRAITX_VULKAN_GRAPHICS_CONTEXT_IMPL_HPP