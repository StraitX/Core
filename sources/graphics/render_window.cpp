#include "graphics/render_window.hpp"

RenderWindow::RenderWindow(u32 width, u32 height, const char* title):
	Window(width, height, title),
	FramebufferChain(this)
{}

void RenderWindow::AcquireNextFramebuffer(const Semaphore* signal) {
	AcquireNext(signal);
}

const Framebuffer* RenderWindow::CurrentFramebuffer()const {
	return FramebufferChain::CurrentFramebuffer();
}

void RenderWindow::PresentCurrentFramebuffer(const Semaphore* wait) {
	PresentCurrent(wait);
}

const RenderPass* RenderWindow::FramebufferPass()const {
	return Pass();
}