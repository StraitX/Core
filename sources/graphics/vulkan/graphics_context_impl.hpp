#ifndef STRAITX_VULKAN_GRAPHICS_CONTEXT_IMPL_HPP
#define STRAITX_VULKAN_GRAPHICS_CONTEXT_IMPL_HPP

#include "core/raw_var.hpp"
#include "graphics/graphics_context.hpp"
#include "graphics/vulkan/swapchain.hpp"
#include "graphics/vulkan/command_buffer.hpp"
#include "graphics/vulkan/fence.hpp"
#include "graphics/vulkan/semaphore.hpp"

namespace Vk{

class GraphicsPipelineImpl;

struct CommandBufferExectuionState{
	const Vk::GraphicsPipelineImpl *PipelineBindPoint = nullptr;
	const Vk::FramebufferImpl *RenderPassFramebuffer = nullptr;
	const Vk::RenderPassImpl *RenderPass = nullptr;

	void Bind(const GraphicsPipeline *pipeline);

	void BeginRenderPass(const class RenderPass *render_pass, const Framebuffer *framebuffer);

	void EndRenderPass();
};

class GraphicsContextImpl: public GraphicsContext{
private:
	VkInstance m_Instance = VK_NULL_HANDLE;
	VkDebugUtilsMessengerEXT m_Messenger = VK_NULL_HANDLE;

	RawVar<Vk::Swapchain> m_Swapchain;
	RawVar<Vk::Semaphore> m_PresentSemaphore;

	RawVar<Vk::CommandBuffer> m_CommandBuffer;

	RawVar<Vk::Fence> m_PrevOpFence;

public:
	static GraphicsContextImpl s_Instance;
public:
	virtual Result Initialize(Window *window)override;

	virtual void Finalize()override;

	virtual void ExecuteCmdBuffer(const GPUCommandBuffer &cmd_buffer)override;

	virtual void SwapBuffers()override;

	virtual void ResizeSwapchain(u32 width, u32 height)override;

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

#endif//STRAITX_VULKAN_GRAPHICS_CONTEXT_IMPL_HPP