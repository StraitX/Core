#include "platform/memory.hpp"
#include "core/log.hpp"
#include "core/string.hpp"
#include "graphics/vulkan/graphics_context_impl.hpp"
#include "graphics/vulkan/gpu.hpp"
#include "graphics/vulkan/dma_impl.hpp"
#include "graphics/vulkan/debug.hpp"
#include "graphics/vulkan/texture_impl.hpp"
#include "graphics/vulkan/descriptor_set_impl.hpp"
#include "graphics/vulkan/graphics_pipeline_impl.hpp"

namespace Vk{

void CommandBufferExectuionState::Bind(const GraphicsPipeline *pipeline){
	PipelineBindPoint = static_cast<const Vk::GraphicsPipelineImpl*>(pipeline);
}

void CommandBufferExectuionState::BeginRenderPass(const class RenderPass *render_pass, const Framebuffer *framebuffer){
	RenderPass = static_cast<const Vk::RenderPassImpl*>(render_pass);
	RenderPassFramebuffer = static_cast<const Vk::FramebufferImpl*>(framebuffer);
}

void CommandBufferExectuionState::EndRenderPass(){
	RenderPass = nullptr;
	RenderPassFramebuffer = nullptr;
}

void CommandBufferExectuionState::CheckAndUpdateViewportAndScissor(VkCommandBuffer cmd_buffer){
	if(ShouldSetScissor){
		VkRect2D rect;
		rect.offset.x = PendingScissor.offset.x;
		rect.offset.y = RenderPassFramebuffer->Size().y - (PendingScissor.offset.y + PendingScissor.extent.height);
		rect.extent.width = PendingScissor.extent.width;
		rect.extent.height = PendingScissor.extent.height;
		vkCmdSetScissor(cmd_buffer, 0, 1, &rect);
	}

	if(ShouldSetViewport){
		VkViewport viewport;
		viewport.minDepth = 0.0;
		viewport.maxDepth = 1.0;
		viewport.x = PendingViewport.x;
		viewport.y = RenderPassFramebuffer->Size().y - PendingViewport.y;
		viewport.width  = PendingViewport.width;
		viewport.height = -PendingViewport.height;
		vkCmdSetViewport(cmd_buffer, 0, 1, &viewport);
	}
}

void CommandBufferExectuionState::UpdateViewport(i32 x, i32 y, u32 width, u32 height){
	PendingViewport.x = x;
	PendingViewport.y = y;
	PendingViewport.width = width;
	PendingViewport.height = height;
	PendingViewport.minDepth = 0.0f;
	PendingViewport.maxDepth = 1.0f;

	ShouldSetViewport = true;
}

void CommandBufferExectuionState::UpdateScissor(i32 x, i32 y, u32 width, u32 height){
	PendingScissor.offset.x = x;
	PendingScissor.offset.y = y;
	PendingScissor.extent.width = width;
	PendingScissor.extent.height = height;

	ShouldSetScissor = true;
}

static_assert(sizeof(GPUHandle) == sizeof(VkPhysicalDevice), "GPUHandle is not the same size as VkPhysicalDevice");
static_assert(sizeof(GPUResourceHandle) == sizeof(VkBuffer), "Vulkan handle is not the same size as GPUResourceHandle");

GraphicsContextImpl GraphicsContextImpl::s_Instance;

Result GraphicsContextImpl::Initialize(){
    Version version = VulkanVersion;
    Span<const char *>extensions = {RequiredPlatformExtensions, RequiredPlatformExtensionsCount};
    Span<const char *>layers     = {RequiredPlatformLayers, RequiredPlatformLayersCount};


    if(!CheckExtensions(extensions))
        return Result::Unsupported;
    if(!CheckLayers(layers))
        return Result::Unsupported;
    
    VkApplicationInfo app_info;
    app_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    app_info.pNext = nullptr;
    app_info.apiVersion = VK_MAKE_VERSION(version.Major, version.Minor, version.Patch);
    app_info.applicationVersion = VK_MAKE_VERSION(0, 0, 1);
    app_info.pApplicationName = "StraitXClient";
    app_info.engineVersion = VK_MAKE_VERSION(0, 0, 1);
    app_info.pEngineName = "StraitX";
    

    VkInstanceCreateInfo info;
    info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    info.pNext = nullptr;
    info.flags = 0;
    info.pApplicationInfo = &app_info;
    info.enabledExtensionCount = extensions.Size();
    info.ppEnabledExtensionNames = extensions.Pointer();
    info.enabledLayerCount = layers.Size();
    info.ppEnabledLayerNames = layers.Pointer();
    
    if(vkCreateInstance(&info, nullptr, &m_Instance) != VK_SUCCESS)
        return Result::Failure;

    VkDebugUtilsMessengerCreateInfoEXT debug_info{};
    debug_info.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    debug_info.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
    debug_info.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
    debug_info.pfnUserCallback = DebugCallback;
    debug_info.pUserData = nullptr; // Optional

    if(CreateDebugUtilsMessengerEXT(m_Instance, &debug_info, nullptr, &m_Messenger) != VK_SUCCESS)
        return Result::Failure;

    if(!GPU::Get().Initialize(PickBestPhysicalDevice(m_Instance)))
        return Result::Failure;

    DMAImpl::Initialize();

	m_Swapchain.Construct(nullptr);
	m_PresentSemaphore.Construct();

	m_CommandBuffer.Construct(QueueFamily::Graphics);

	m_PrevOpFence.Construct();

	m_Swapchain->AcquireNext(m_PresentSemaphore->Handle, m_PrevOpFence->Handle);

	return Result::Success;
}

void GraphicsContextImpl::Finalize(){
	m_PrevOpFence->WaitAndReset();

	m_CommandBuffer.Destruct();

	m_Swapchain->PresentCurrent({&m_PresentSemaphore->Handle, 1});
	m_Swapchain.Destruct();

	m_PresentSemaphore.Destruct();
	m_PrevOpFence.Destruct();

	DMAImpl::Finalize();

    GPU::Get().Finalize();

    DestroyDebugUtilsMessengerEXT(m_Instance, m_Messenger, nullptr);
    vkDestroyInstance(m_Instance, nullptr);
}

void GraphicsContextImpl::ExecuteCmdBuffer(const GPUCommandBuffer &cmd_buffer){
	VkCommandBuffer VkCmdBuffer = m_CommandBuffer->Handle();

	CommandBufferExectuionState state;

	m_CommandBuffer->Begin();

	for(const auto &cmd: cmd_buffer){
		switch(cmd.Type){
		case GPUCommandType::None:
		{
			(void)0;
		}
		break;
		case GPUCommandType::CopyCPUToGPUBuffer: 
		{
			VkBuffer src = VkBuffer(cmd.CopyCPUToGPUBuffer.Source.U64);
			VkBuffer dst = VkBuffer(cmd.CopyCPUToGPUBuffer.Destination.U64);

			m_CommandBuffer->CmdBufferCopy(
				src,
				dst,
				cmd.CopyCPUToGPUBuffer.Size,
				cmd.CopyCPUToGPUBuffer.SourceOffset,
				cmd.CopyCPUToGPUBuffer.DestinationOffset
			);
																					// XXX this can be optimized
			m_CommandBuffer->CmdMemoryBarrier(VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT, VK_ACCESS_MEMORY_WRITE_BIT, VK_ACCESS_MEMORY_READ_BIT);
		}
		break;
		case GPUCommandType::ChangeTextureLayout: 
		{

			m_CommandBuffer->CmdImageBarrier(
				VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT, // XXX this can be optimized
				VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, 
				VK_ACCESS_MEMORY_WRITE_BIT, 
				VK_ACCESS_MEMORY_WRITE_BIT | VK_ACCESS_MEMORY_READ_BIT, 
				TextureImpl::s_LayoutTable[(size_t)cmd.ChangeTextureLayout.OldLayout], 
				TextureImpl::s_LayoutTable[(size_t)cmd.ChangeTextureLayout.NewLayout], 
				VkImage(cmd.ChangeTextureLayout.Texture->Handle().U64)
			);
			cmd.ChangeTextureLayout.Texture->m_Layout = cmd.ChangeTextureLayout.NewLayout;
		}
		break;
		case GPUCommandType::BindPipeline: 
		{
			state.Bind(cmd.BindPipeline.Pipeline);
			vkCmdBindPipeline(VkCmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, state.PipelineBindPoint->Handle);
		}		
		break;
		case GPUCommandType::BindDescriptorSet: 
		{
			VkDescriptorSet set_handle = ((const Vk::DescriptorSetImpl*)cmd.BindDescriptorSet.DescriptorSet)->Handle();

			vkCmdBindDescriptorSets(VkCmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, state.PipelineBindPoint->Layout, 0, 1, &set_handle, 0, nullptr);
		}		
		break;
		case GPUCommandType::BindVertexBuffer: 
		{
			VkBuffer vb = VkBuffer(cmd.BindVertexBuffer.VertexBuffer.U64);
			VkDeviceSize offset = 0;
			vkCmdBindVertexBuffers(VkCmdBuffer, 0, 1, &vb, &offset);
		}		
		break;
		case GPUCommandType::BindIndexBuffer: 
		{
			static VkIndexType s_IndexType[]={
    			VK_INDEX_TYPE_UINT16,
    			VK_INDEX_TYPE_UINT32
			};

			VkBuffer ib = VkBuffer(cmd.BindIndexBuffer.IndexBuffer.U64);
			VkIndexType it = s_IndexType[(size_t)cmd.BindIndexBuffer.IndicesType];

			vkCmdBindIndexBuffer(VkCmdBuffer, ib, 0, it);
		}		
		break;
		case GPUCommandType::BeginRenderPass: 
		{
			state.BeginRenderPass(cmd.BeginRenderPass.RenderPass, cmd.BeginRenderPass.Framebuffer);

			VkRenderPassBeginInfo info;
			info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
			info.pNext = nullptr;
			info.clearValueCount = 0;
			info.pClearValues = nullptr;
			info.framebuffer = state.RenderPassFramebuffer->Handle();
			info.renderPass = state.RenderPass->Handle();
			info.renderArea.offset = {0, 0};
			info.renderArea.extent.width = state.RenderPassFramebuffer->Size().x;
			info.renderArea.extent.height = state.RenderPassFramebuffer->Size().y;
			
			vkCmdBeginRenderPass(VkCmdBuffer, &info, VK_SUBPASS_CONTENTS_INLINE);
		}		
		break;
		case GPUCommandType::EndRenderPass: 
		{
    		vkCmdEndRenderPass(VkCmdBuffer);

			state.EndRenderPass();
		}		
		break;
		case GPUCommandType::DrawIndexed: 
		{
			state.CheckAndUpdateViewportAndScissor(VkCmdBuffer);
    		vkCmdDrawIndexed(VkCmdBuffer, cmd.DrawIndexed.IndicesCount, 1, cmd.DrawIndexed.IndexOffset, 0, 0);
		}		
		break;
		case GPUCommandType::ClearFramebufferColorAttachments:
		{
			auto fb_impl = static_cast<const Vk::FramebufferImpl*>(cmd.ClearFramebufferColorAttachments.Framebuffer);

			Vector4f color = cmd.ClearFramebufferColorAttachments.Color;

			VkClearColorValue value;
			value.float32[0] = color[0];
			value.float32[1] = color[1];
			value.float32[2] = color[2];
			value.float32[3] = color[3];

			VkImageSubresourceRange issr;
			issr.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			issr.baseArrayLayer = 0;
			issr.baseMipLevel = 0;
			issr.levelCount = 1;
			issr.layerCount = 1;

			for(auto &att: fb_impl->Attachments()){
				if(IsColorFormat(att->Format())){

					auto layout = TextureImpl::s_LayoutTable[(size_t)att->Layout()];
					constexpr auto clear_layout = VK_IMAGE_LAYOUT_GENERAL;

					m_CommandBuffer->CmdImageBarrier(VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT, VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, VK_ACCESS_MEMORY_WRITE_BIT, VK_ACCESS_MEMORY_WRITE_BIT, layout, clear_layout, (VkImage)att->Handle().U64);

					vkCmdClearColorImage(VkCmdBuffer, (VkImage)att->Handle().U64, clear_layout, &value, 1, &issr);

					m_CommandBuffer->CmdImageBarrier(VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT, VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT, VK_ACCESS_MEMORY_WRITE_BIT, VK_ACCESS_MEMORY_READ_BIT | VK_ACCESS_MEMORY_READ_BIT, clear_layout, layout, (VkImage)att->Handle().U64);
				}
			}
		}
		break;
		case GPUCommandType::ClearFramebufferDepthAttachments:
		{
			auto fb_impl = static_cast<const Vk::FramebufferImpl*>(cmd.ClearFramebufferDepthAttachments.Framebuffer);

			VkClearDepthStencilValue depth_value;
			depth_value.depth = cmd.ClearFramebufferDepthAttachments.Depth;
			depth_value.stencil = 0;

			VkImageSubresourceRange issr;
			issr.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
			issr.baseArrayLayer = 0;
			issr.baseMipLevel = 0;
			issr.levelCount = 1;
			issr.layerCount = 1;

			for(auto &att: fb_impl->Attachments()){
				if(IsDepthFormat(att->Format())){

					auto layout = TextureImpl::s_LayoutTable[(size_t)att->Layout()];
					constexpr auto clear_layout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;

					m_CommandBuffer->CmdImageBarrier(VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT, VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, VK_ACCESS_MEMORY_WRITE_BIT, VK_ACCESS_MEMORY_WRITE_BIT, layout, clear_layout, (VkImage)att->Handle().U64, VK_IMAGE_ASPECT_DEPTH_BIT);

					vkCmdClearDepthStencilImage(VkCmdBuffer, (VkImage)att->Handle().U64, clear_layout, &depth_value, 1, &issr);

					m_CommandBuffer->CmdImageBarrier(VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT, VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT, VK_ACCESS_MEMORY_WRITE_BIT, VK_ACCESS_MEMORY_READ_BIT | VK_ACCESS_MEMORY_READ_BIT, clear_layout, layout, (VkImage)att->Handle().U64, VK_IMAGE_ASPECT_DEPTH_BIT);
				}
			}
		}
		break;
		case GPUCommandType::SetScissors:
		{
			auto &cmd_scissors = cmd.SetScissors;
			state.UpdateScissor(cmd_scissors.x, cmd_scissors.y, cmd_scissors.Width, cmd_scissors.Height);
		}
		break;
		case GPUCommandType::SetViewport:
		{
			auto &cmd_viewport = cmd.SetViewport;

			state.UpdateViewport(cmd_viewport.x, cmd_viewport.y, cmd_viewport.Width, cmd_viewport.Height);
		}
		break;
		}
	}

	m_CommandBuffer->End();

	m_PrevOpFence->WaitAndReset();

	m_CommandBuffer->Submit({}, {}, m_PrevOpFence->Handle);

	m_PrevOpFence->Wait();
}

void GraphicsContextImpl::SwapBuffers(){
	m_PrevOpFence->WaitAndReset();

	if(m_Swapchain->Size() != Vector2u(PlatformWindow::Size())){

		m_Swapchain.Destruct();
		m_Swapchain.Construct(PlatformWindow());
		//recreate semaphore to reset it
		m_PresentSemaphore.Destruct();
		m_PresentSemaphore.Construct();

		m_Swapchain->AcquireNext(m_PresentSemaphore->Handle, m_PrevOpFence->Handle);
	}else{
		m_Swapchain->PresentCurrent({&m_PresentSemaphore->Handle, 1});
		m_Swapchain->AcquireNext(m_PresentSemaphore->Handle, m_PrevOpFence->Handle);
	}
}

void GraphicsContextImpl::ResizeSwapchain(u32 width, u32 height){
	(void)width;
	(void)height;

	// according to the vulkan spec,  window size must be the same as swapchain image size
	// during vkQueuePresentKHR() and vkAcquireNextImageKHR() calls
	// so we have to resize swapchain before swapbuffers,
	// now this function serers as nop on Vulkan
}

const Framebuffer *GraphicsContextImpl::CurrentFramebuffer(){
	return m_Swapchain->CurrentFramebuffer();
}

const RenderPass *GraphicsContextImpl::FramebufferPass(){
	return m_Swapchain->FramebufferPass();
}

GPUType GraphicsContextImpl::VkTypeToGPUType(u32 type){
    switch (type) {
    case VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU:      return GPUType::Discrete;
    case VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU:    return GPUType::Integrated;
    case VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU:       return GPUType::Virtual;
    case VK_PHYSICAL_DEVICE_TYPE_OTHER:             return GPUType::Other;
    default:
        return GPUType::Unknown;
    }
}

int GraphicsContextImpl::GetPriority(GPUType type){
    switch (type) {
    case GPUType::Unknown: return 0;
    case GPUType::Other: return 0;
    case GPUType::Virtual: return 1;
    case GPUType::Integrated: return 2;
    case GPUType::Discrete: return 3;
    }
}

VkPhysicalDevice GraphicsContextImpl::PickBestPhysicalDevice(VkInstance instance){
    u32 count = 0;
    vkEnumeratePhysicalDevices(instance, &count, nullptr);

    auto *devices = SX_STACK_ARRAY_ALLOC(VkPhysicalDevice, count);

    vkEnumeratePhysicalDevices(instance, &count, devices);

    VkPhysicalDevice best = devices[0];
    int best_score = 0;
    
    for(u32 i = 0; i<count; i++){
        VkPhysicalDeviceProperties props;
        vkGetPhysicalDeviceProperties(devices[i], &props);

        u32 families_count = 0;
        vkGetPhysicalDeviceQueueFamilyProperties(devices[i], &families_count, nullptr);

        int score = GetPriority(VkTypeToGPUType(props.deviceType))*10 + families_count * 5; 

        if(score > best_score){
            best = devices[i];
            best_score = score;
        }
    }

    return best;
}

bool GraphicsContextImpl::CheckLayers(const Span<const char *> &layers){
    u32 count = 0;
    vkEnumerateInstanceLayerProperties(&count, nullptr);
    VkLayerProperties *props = (VkLayerProperties*)alloca(count * sizeof(VkLayerProperties));
    vkEnumerateInstanceLayerProperties(&count, props);

    for(auto &lay: layers){
        bool supported = false;
        for(int i = 0; i<count; i++){
            if(String::Equals(lay,props[i].layerName))
                supported = true;
        }
        if(!supported){
            LogError("Vulkan: Layer % is not supported",lay);
            return false;
        }
    }

    return true;
}

bool GraphicsContextImpl::CheckExtensions(const Span<const char *> &extensions){
    u32 count = 0;
    vkEnumerateInstanceExtensionProperties(nullptr, &count, nullptr);
    VkExtensionProperties *props = (VkExtensionProperties *)alloca(count * sizeof(VkExtensionProperties));
    vkEnumerateInstanceExtensionProperties(nullptr, &count, props);
    
    for(auto &ext: extensions){
        bool supported = false;
        for(int i = 0; i<count; i++){
            if(String::Equals(ext,props[i].extensionName))
                supported = true;
        }
        if(!supported){
            LogError("Vulkan: Extension % is not supported",ext);
            return false;
        }
    }

    return true; 
}

}//namespace Vk::