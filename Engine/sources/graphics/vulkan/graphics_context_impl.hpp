#ifndef STRAITX_VULKAN_GRAPHICS_CONTEXT_IMPL_HPP
#define STRAITX_VULKAN_GRAPHICS_CONTEXT_IMPL_HPP

#include "graphics/api/graphics_context.hpp"
#include "graphics/vulkan/swapchain_impl.hpp"

namespace StraitX{
namespace Vk{

class GraphicsContextImpl: public GraphicsContext{
private:
	VkInstance m_Instance = VK_NULL_HANDLE;
	VkDebugUtilsMessengerEXT m_Messenger = VK_NULL_HANDLE;

	Vk::SwapchainImpl *m_Swapchain = nullptr;
public:
	static GraphicsContextImpl s_Instance;
public:
	virtual Result Initialize(const Window &window)override;

	virtual void Finalize()override;
	//virtual void ScheduleCmdBuffer(CmdBuffer cmd_buffer) = 0;

	virtual void BeginFrame()override;

	//calls ExecuteCmdBuffers()
	virtual void EndFrame()override;

	//virtual void ExecuteScheduledCmdBuffers() = 0;

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