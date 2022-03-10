#ifndef STRAITX_RENDER_WINDOW_HPP
#define STRAITX_RENDER_WINDOW_HPP

#include "core/os/window.hpp"
#include "graphics/api/swapchain.hpp"

class RenderWindow : public Window, private FramebufferChain {
public:
	RenderWindow(u32 width, u32 height, const char *title = "RenderWindow", TextureFormat depth_buffer_format = TextureFormat::Unknown);

	void AcquireNextFramebuffer(const Semaphore *signal);

	const Framebuffer *CurrentFramebuffer()const;

	void PresentCurrentFramebuffer(const Semaphore *wait);

	const RenderPass *FramebufferPass()const;
};

#endif//STRAITX_RENDER_WINDOW_HPP